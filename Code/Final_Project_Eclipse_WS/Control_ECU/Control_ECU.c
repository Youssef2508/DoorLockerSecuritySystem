/*******************************************************************************
 *
 * Module: Control Application
 *
 * File Name: control_application.c
 *
 * Description: Source file for the Control ECU application layer.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "std_types.h"
#include "uart.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "pir.h"
#include "twi.h"
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include "timer.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE				5
#define PASSWORD_SAVED 				0x11
#define DIFF_PASSWORDS				0x22
#define TRUE_PASSWORD				0x33
#define WRONG_PASSWORD				0x32
#define CONTROL_ECU_READY			0xFF
#define LOCKING_DOOR				0x44
#define UNLOCK_DOOR    				0x55
#define CHANGE_PASSWORD				0xE3


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint8 tick = 0;
volatile boolean t_15s_flag = FALSE;
volatile boolean t_60s_flag = FALSE;
volatile boolean alarm_flag = FALSE;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void getAndSavePassword(void);
void timerCallBack(void);


/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/

int main(void){
	uint8 size = PASSWORD_SIZE + 1;
	uint8 enteredPass[size] , savedPass[size];
	uint8 action = 0;
	/* Create configuration structure for timer driver */
	Timer_ConfigType timerConfig = {0, 39062, TIMER1_ID, TIMER_F_CPU_1024, TIMER_COMPARE_MODE};
	/* Create configuration structure for UART driver */
	UART_ConfigType uartConfig = {9600, UART_PARITY_DISABLED, UART_ONE_STOP_BIT, UART_8_BIT_DATA, UART_POLLING_MODE};

	/* Enable Global Interrupt */
	SREG |= (1 << 7);

	/* Initialize the UART driver with :
	 * Baud-rate = 9600 bits/sec
	 * one stop bit
	 * No parity
	 * 8-bit data
	 */
	UART_init(&uartConfig);

	/* Initialize the TWI driver with :
	 * my address = 0x01
	 * bite rate = 0x02    so SCL frequency= 400 bit/s
	 */
	TWI_init();

	/* Initialize the Buzzer */
	Buzzer_init();
	/* Initialize the DC Motor */
	DcMotor_Init();
	/* Initialize the PIR Sensor */
	PIR_init();

	/* Get the password from HMI_ECU and save it in the External EEPRPOM */
	getAndSavePassword();
	_delay_ms(10);

	while(1) {
		uint8 i;

		/* loop 3 times until the user enter the true password */
		for(i = 0 ; i < 3 ; i++){
			/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the password */
			UART_sendByte(CONTROL_ECU_READY);
			/* Receive the password from HMI_ECU through UART */
			UART_receiveString(enteredPass);

			/* get the password saved in the EEPROM */
			EEPROM_readData(0x0311, savedPass, PASSWORD_SIZE);
			savedPass[PASSWORD_SIZE] = '\0';
			/* compare the received password and the saved password */
			if(!strcmp((char*)enteredPass,(char*)savedPass)){
				/* if the two passwords are the same send TRUE_PASSWORD byte to HMI_ECU */
				UART_sendByte(TRUE_PASSWORD);
				/* Receive an action byte from HMI_ECU (Open Door or Change Password) */
				action = UART_receiveByte();
				break;
			}else{
				/* if the two passwords are not the same send WRONG_PASSWORD byte to HMI_ECU */
				UART_sendByte(WRONG_PASSWORD);
			}
		}

		/* if loop_counter variable reach 3, this means the user enters wrong password 3 times */
		if(i == 3){
			/* Set alarm flag */
			alarm_flag = TRUE;
			/* Set the Call back function pointer in the timer driver */
			Timer_setCallBack(timerCallBack, TIMER1_ID);
			/* Initialize the Timer driver :
			 * use timer 1
			 * prescaler 1024
			 * compare mode
			 * initial value = 0
			 * compare value = 39062, so the interrupt occurs every 5 seconds
			 */
			Timer_init(&timerConfig);

			/* turn on buzzer */
			Buzzer_on();
			/* wait until t_60s_flag become true, this will happen after 1 minute */
			while(t_60s_flag != TRUE);
			/* Reset the flags */
			t_60s_flag = FALSE;
			alarm_flag = FALSE;
			/* turn off buzzer */
			Buzzer_off();
		}
		/* if loop_counter variable doesn't reach 3, then the user enter the true password */
		else{
			/* process Open Door option */
			if(action == UNLOCK_DOOR){
				/* Set the call back function and initialize timer driver again  */
				Timer_setCallBack(timerCallBack,TIMER1_ID);
				Timer_init(&timerConfig);

				/* Rotate the motor clockwise with maximum speed for 15 seconds */
				DcMotor_Rotate(CW, 100);
				/* wait until t_15s_flag become true, this will happen after 15 seconds */
				while(t_15s_flag != TRUE);
				/* reset the flag */
				t_15s_flag = FALSE;

				/* stop the motor to keep the door open */
				DcMotor_Rotate(CW, 0);
				/* wait until PIR sensor detect no motion (wait for all people to enter)*/
				while(PIR_getState());

				/* send LOCKING_DOOR byte to HMI_ECU */
				UART_sendByte(LOCKING_DOOR);

				/* Set the call back function and initialize timer driver again  */
				Timer_setCallBack(timerCallBack,TIMER1_ID);
				Timer_init(&timerConfig);

				/* Rotate the motor anti-clockwise with maximum speed for 15 seconds */
				DcMotor_Rotate(A_CW, 100);
				/* wait until t_15s_flag become true, this will happen after 15 seconds */
				while(t_15s_flag != TRUE);
				/* reset the flag */
				t_15s_flag = FALSE;
				/* stop the motor */
				DcMotor_Rotate(CW, 0);
			}
			/* process Change Password option */
			else if(action == CHANGE_PASSWORD){

				/* Get the password from HMI_ECU and save it in the External EEPRPOM */
				getAndSavePassword();
				_delay_ms(10);
			}
		}

	}
}


/*
 * Description :
 * Function responsible for Get the password from HMI_ECU and save it in the External EEPRPOM.
 */
void getAndSavePassword(void){
	uint8 size = PASSWORD_SIZE + 1;
	uint8 pass1[size], pass2[size];
	/* loop until the user enters same password twice for confimation  */
	while(1) {
		/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the two passwords */
		UART_sendByte(CONTROL_ECU_READY);
		/* Receive the password and the confirmation password from HMI_ECU */
		UART_receiveString(pass1);
		UART_receiveString(pass2);

		/* compare the two passwords */
		if(!strcmp((char *)pass1 , (char *)pass2)){
			/* if the two passwords are the same save the password in the EEPROM */
			EEPROM_writeData(0x0311, pass1, PASSWORD_SIZE);
			/* send PASSWORD_SAVED byte to HMI_ECU */
			UART_sendByte(PASSWORD_SAVED);
			return;
		}else{
			/* if the two passwords are not the same, send DIFF_PASSWORDS byte to HMI_ECU */
			UART_sendByte(DIFF_PASSWORDS);
		}
	}
}


/*
 * Description :
 * call-back function.
 */
void timerCallBack(void){
	tick++;
	if((!alarm_flag) && (tick == 3)){
		t_15s_flag = TRUE;
		tick = 0;

		Timer_deInit(TIMER1_ID);

	}else if((alarm_flag) && (tick == 12)){
		t_60s_flag = TRUE;
		tick = 0;

		Timer_deInit(TIMER1_ID);
	}
}



