/*******************************************************************************
 *
 * Module: HMI Application
 *
 * File Name: hmi_application.c
 *
 * Description: Source file for the HMI application layer.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include <avr/io.h>
#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include <util/delay.h>
#include "uart.h"
#include "timer.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE 				5
#define PASSWORD_SAVED 				0x11
#define DIFF_PASSWORDS				0x22
#define TRUE_PASSWORD				0x33
#define WRONG_PASSWORD				0x32
#define CONTROL_ECU_READY			0xFF
#define LOCKING_DOOR				0x44
#define UNLOCK_DOOR    				0x55
#define ALARM_MODE					0x53
#define CHANGE_PASSWORD				0xE3
#define Enter                       13


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint8 tick = 0;
volatile boolean t_15s_flag = FALSE;
volatile boolean t_60s_flag = FALSE;
volatile boolean alarm_flag = FALSE;
/* Create configuration structure for timer driver */
Timer_ConfigType timerConfig = {0, 39062, TIMER1_ID, TIMER_F_CPU_1024, TIMER_COMPARE_MODE};


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void createPassword(void);
void getPassword(uint8 * pass, uint8 size);
void checkPassword(uint8* isPassTrue);
void alarmMode(void);
void timerCallBack(void);


/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/

int main(void){
	uint8 key;
	uint8 isPassTrue;
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
	/* Initialize the LCD */
	LCD_init();

	/* when the system start for the first time, create New Password */
	createPassword();

	LCD_clearScreen();

	while(1) {
		/* Display always the main system options */
		LCD_displayString("+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change Pass");

		/* Get the key pressed by user */
		key = KEYPAD_getPressedKey();

		/* if user chooses (+) Open Door */
		if(key == '+'){
			/* The user should enter the password saved in EEPROM */
			checkPassword(&isPassTrue);
			/* if the user entered the true password */
			if (isPassTrue == TRUE_PASSWORD) {
				/* Send UNLOCK_DOOR to the Control ECU to open the Door (rotate motor) */
				UART_sendByte(UNLOCK_DOOR);
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

				/* Display Door Unlocking please wait on LCD for 15 seconds */
				LCD_clearScreen();
				LCD_displayString("Door Unlocking");
				LCD_displayStringRowColumn(1, 0,  "please wait");
				 /* wait until t_15s_flag become true, this will happen after 15 seconds */
				while (t_15s_flag != TRUE);
				/* reset the flag */
				t_15s_flag = FALSE;

				/* Display wait for people to enter */
				LCD_clearScreen();
				LCD_displayString("Wait for people");
				LCD_displayStringRowColumn(1, 6, "to enter");
				/* wait until Contro ECU sends LOCKING_DOOR */
				while (UART_receiveByte() != LOCKING_DOOR);

				/* Set the call back function and initialize timer driver again  */
				Timer_setCallBack(timerCallBack, TIMER1_ID);
				Timer_init(&timerConfig);

				/* Display Door Locking on LCD for 15 seconds */
				LCD_clearScreen();
				LCD_displayString("Door Locking");
				/* wait until t_15s_flag become true, this will happen after 15 seconds */
				while (t_15s_flag != TRUE);
				/* reset the flag */
				t_15s_flag = FALSE;
				LCD_clearScreen();
			}
			/* if the user entered wrong password 3 times */
			else if(isPassTrue == WRONG_PASSWORD){
				/* enter alarm mode for 1 minute */
				alarmMode();
			}

		}
		/* if user chooses (-) Change Password */
		else if(key == '-'){
			/* The user should enter the password saved in EEPROM */
			checkPassword(&isPassTrue);

			/* if the user entered the true password */
			if(isPassTrue == TRUE_PASSWORD){
				/* Send CHANGE_PASSWORD to the Control ECU to get ready to save new password */
				UART_sendByte(CHANGE_PASSWORD);
				/* Create new password */
				createPassword();
				LCD_clearScreen();
			}
			/* if the user entered wrong password 3 times */
			else if(isPassTrue == WRONG_PASSWORD){
				/* enter alarm mode for 1 minute */
				alarmMode();
			}
		}

	}
}

/*
 * Description :
 * Function responsible for :
 * 1. Display error message on LCD for 1 minute
 * 2. Wait for 1 minute and no inputs from the keypad will be accepted during this time period.
 */
void alarmMode(void){
	/* Set alarm flag */
	alarm_flag = TRUE;

	/* Set the call back function and initialize timer driver again  */
	Timer_setCallBack(timerCallBack, TIMER1_ID);
	Timer_init(&timerConfig);

	/* Display error message on LCD */
	LCD_clearScreen();
	LCD_displayString("System LOCKED");
	LCD_displayStringRowColumn(1, 0, "Wait for 1 min");

	/* wait until t_60s_flag become true, this will happen after 1 minute */
	while(t_60s_flag != TRUE);
	/* reset the flag */
	t_60s_flag = FALSE;
	/* Reset alarm flag */
	alarm_flag = FALSE;
	LCD_clearScreen();
}

/*
 * Description :
 * Function responsible for :
 * 1. get the password from the user.
 * 2. send it to Control ECU.
 * 3. set a flag to  TRUE_PASSWORD if the user enters the true password
 *    or to WRONG_PASSWORD if the user enters a wrong password.
 */
void checkPassword(uint8* flag_ptr){
	uint8 size = PASSWORD_SIZE + 2;
	uint8 pass[size] , i;

	/* loop 3 times each time get password from the user it true return */
	for(i = 0 ; i < 3 ; i++){
		LCD_clearScreen();
		LCD_displayString("Enter your pass:");
		LCD_moveCursor(1,0);

		/* Get the password from the user */
		getPassword(pass, size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != Enter);

		/* Wait until Control_ECU is ready to receive the password */
		while(UART_receiveByte() != CONTROL_ECU_READY);
		/* send the password */
		UART_sendString(pass);

		/* if the password is true Control_ECU will send TRUE_PASSWORD , if not it will sends WRONG_PASSWORD */
		*flag_ptr = UART_receiveByte();
		if(*flag_ptr == TRUE_PASSWORD){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 1, "Right Password");
			_delay_ms(1000);
			return;
		}
		else{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Wrong Password!");
			_delay_ms(1000);
		}
	} /* it will reach here if the user enters wrong password 3 times and then (*flag_ptr=WRONG_PASSWORD)  */
}

/*
 * Description :
 * Function responsible for create new password by:
 * 1. take the password from the user and take the password again for confirmation.
 * 2. send the two passwords to Control_ECU.
 * 3. it will repeat until the user enters the same password twice
 */
void createPassword(void){
	uint8 size = PASSWORD_SIZE + 2;
	uint8 pass1[size], pass2[size];
	uint8 match;
	/* loop until the user enters same password twice for confirmation */
	while(match != PASSWORD_SAVED) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "plz enter pass:");
		LCD_moveCursor(1, 0);

		/* Get the password from the user */
		getPassword(pass1, size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != Enter);

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "plz re-enter the");
		LCD_displayStringRowColumn(1, 0, "same pass:");

		/* Get the password again from the user for confirmation */
		getPassword(pass2, size);
		/* wait until the user press enter button */
		while(KEYPAD_getPressedKey() != Enter);

		/* Wait until Control_ECU is ready to receive the password */
		while(UART_receiveByte() != CONTROL_ECU_READY);
		/* send the to passwords to Control_ECU */
		UART_sendString(pass1);
		UART_sendString(pass2);

		/* if the two passwords are the same Control_ECU will save the password in the EEPROM and send PASSWORD_SAVED */
		match = UART_receiveByte();

		if(match == DIFF_PASSWORDS){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 2, "Not Matched!");
			_delay_ms(1000);
		}
		else{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 5, "Matched");
			_delay_ms(1000);
		}
	}

}

/*
 * Description :
 * Function responsible for take password from user.
 */
void getPassword(uint8 * pass, uint8 size){
	uint8 i;
	for (i = 0; i < size - 2; i++) {
		pass[i] = KEYPAD_getPressedKey() + 48;
		LCD_displayCharacter('*');
	}

	/* end the password with # and null */
	pass[i++] = '#';
	pass[i] = '\0';
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




