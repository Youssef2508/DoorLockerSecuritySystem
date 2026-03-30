/******************************************************************************
 *
 * Module: DC Motor Driver
 *
 * File Name: dc_motor_driver.c
 *
 * Description: Source file for controlling the DC motor based on temperature readings.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h" /* Use PWM driver for speed control */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Initializes the DC motor by setting the direction for the motor control pins.
 * Stops the motor initially by setting both IN1 and IN2 to low.
 */
void DcMotor_Init(void)
{
	Timer0_init();
    /* Set the direction for the motor control pins (IN1, IN2, and Enable) as output */
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_ENABLE_PORT_ID, MOTOR_ENABLE_PIN_ID, PIN_OUTPUT);

    /* Initially stop the motor by setting both IN1 and IN2 to low */
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
    GPIO_writePin(MOTOR_ENABLE_PORT_ID, MOTOR_ENABLE_PIN_ID, LOGIC_HIGH);  /* Enable the motor initially */
}

/*
 * Description:
 * Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed
 * using the PWM duty cycle for speed control.
 *
 * Parameters:
 * - state: Enum or uint8 that indicates motor direction (CW, A-CW, Stop).
 * - speed: Motor speed in percentage (0 to 100%).
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
    switch (state)
    {
        case CW:  /* Clockwise rotation */
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
            break;

        case A_CW:  /* Anti-clockwise rotation */
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
            break;

        case STOP:  /* Stop the motor */
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
            break;
    }

    /* Adjust the motor speed using PWM duty cycle */
    PWM_Timer0_Start(speed);  /* Use the PWM driver to adjust the speed */
}
