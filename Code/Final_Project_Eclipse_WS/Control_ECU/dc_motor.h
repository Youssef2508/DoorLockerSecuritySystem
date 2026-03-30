/******************************************************************************
 *
 * Module: DC Motor Driver
 *
 * File Name: dc_motor_driver.h
 *
 * Description: Header file for controlling the DC motor based on temperature readings.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef DC_MOTOR_DRIVER_H_
#define DC_MOTOR_DRIVER_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* DC Motor Pins Configurations: Adjust to match your microcontroller port/pin assignments */
#define MOTOR_PORT_ID          PORTD_ID
#define MOTOR_IN1_PIN_ID       PIN6_ID
#define MOTOR_IN2_PIN_ID       PIN7_ID

#define MOTOR_ENABLE_PORT_ID   PORTB_ID
#define MOTOR_ENABLE_PIN_ID    PIN3_ID

/* Motor Direction Enumeration */
typedef enum
{
    CW,        /* Clockwise */
    A_CW,      /* Anti-Clockwise */
    STOP       /* Stop the motor */
} DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Initializes the DC motor by setting up the direction for the motor control pins
 * (IN1, IN2, and Enable) and stopping the motor initially.
 */
void DcMotor_Init(void);

/*
 * Description:
 * Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed
 * using the PWM duty cycle for speed control.
 *
 * Parameters:
 * - state: Enum or uint8 that indicates motor direction (CW, A-CW, Stop).
 * - speed: Motor speed in percentage (0 to 100%).
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_DRIVER_H_ */
