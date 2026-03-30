/******************************************************************************
 *
 * Module: PIR Sensor
 *
 * File Name: pir.c
 *
 * Description: Source file for the PIR sensor driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "pir.h"
#include "gpio.h" /* For GPIO operations */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Initialize the PIR sensor by configuring its pin as input.
 */
void PIR_init(void)
{
    /* Configure the PIR sensor pin as input */
    GPIO_setupPinDirection(PIR_SENSOR_PORT_ID, PIR_SENSOR_PIN_ID, PIN_INPUT);
}

/*
 * Description:
 * Get the current state of the PIR sensor.
 * Returns: 1 if motion is detected, 0 if no motion.
 */
uint8 PIR_getState(void)
{
    /* Return the logic level of the PIR sensor pin (1: motion detected, 0: no motion) */
    return GPIO_readPin(PIR_SENSOR_PORT_ID, PIR_SENSOR_PIN_ID);
}
