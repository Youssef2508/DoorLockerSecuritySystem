/******************************************************************************
 *
 * Module: Buzzer Driver
 *
 * File Name: buzzer_driver.c
 *
 * Description: Source file for controlling the buzzer based on fire detection.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h" /* Use the GPIO driver for pin configuration and control */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Initializes the buzzer pin direction as an output pin and turns off the buzzer.
 */
void Buzzer_init(void)
{
    /* Set the buzzer pin as an output pin */
    GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);

    /* Turn off the buzzer initially (set pin low) */
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*
 * Description:
 * Activates the buzzer (turns the buzzer ON).
 */
void Buzzer_on(void)
{
    /* Set the buzzer pin high to turn it ON */
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*
 * Description:
 * Deactivates the buzzer (turns the buzzer OFF).
 */
void Buzzer_off(void)
{
    /* Set the buzzer pin low to turn it OFF */
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
