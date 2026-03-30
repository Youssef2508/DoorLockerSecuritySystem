/******************************************************************************
 *
 * Module: PIR Sensor
 *
 * File Name: pir.h
 *
 * Description: Header file for the PIR sensor driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Define the PIR sensor port and pin */
#define PIR_SENSOR_PORT_ID      PORTC_ID
#define PIR_SENSOR_PIN_ID       PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Initialize the PIR sensor by configuring its pin as input.
 */
void PIR_init(void);

/*
 * Description:
 * Get the current state of the PIR sensor.
 * Returns: 1 if motion is detected, 0 if no motion.
 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
