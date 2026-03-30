/******************************************************************************
 *
 * Module: Buzzer Driver
 *
 * File Name: buzzer_driver.h
 *
 * Description: Header file for controlling the buzzer based on fire detection.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef BUZZER_DRIVER_H_
#define BUZZER_DRIVER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Buzzer Pin Configuration:
 * The buzzer is connected to a specific port and pin, which can be configured here.
 */
#define BUZZER_PORT_ID            PORTC_ID
#define BUZZER_PIN_ID             PIN7_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Initializes the buzzer pin direction as an output pin and turns off the buzzer initially.
 */
void Buzzer_init(void);

/*
 * Description:
 * Activates the buzzer (turns the buzzer ON).
 */
void Buzzer_on(void);

/*
 * Description:
 * Deactivates the buzzer (turns the buzzer OFF).
 */
void Buzzer_off(void);

#endif /* BUZZER_DRIVER_H_ */
