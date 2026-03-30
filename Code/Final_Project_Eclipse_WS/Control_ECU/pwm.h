/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the PWM driver
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/

/* PWM Frequency Prescaler */
#define PWM_PRESCALER F_CPU_64

/* Timer/Counter Control Register for Timer0 */
#define PWM_TIMER0_TCCR0 (*(volatile uint8 *)(0x53))
#define PWM_TIMER0_TCNT0 (*(volatile uint8 *)(0x52)) /* Timer/Counter Register */
#define PWM_TIMER0_OCR0 (*(volatile uint8 *)(0x5C))  /* Output Compare Register */

/* Timer/Counter Interrupt Mask Register */
#define PWM_TIMER0_TIMSK (*(volatile uint8 *)(0x59))

/* Output Pin for PWM signal (OC0) */
#define PWM_OUTPUT_PIN PORTB_ID
#define PWM_OUTPUT_PIN_NUMBER PIN3_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description:
 * This function is responsible for initializing Timer0 in PWM mode.
 * It sets up Timer0 in non-inverting mode and starts generating PWM
 * with the desired duty cycle.
 *
 * Parameters:
 *  - duty_cycle: percentage value (0 to 100%) representing the PWM duty cycle.
 */
void PWM_Timer0_Start(uint8 duty_cycle);

void Timer0_init(void);

#endif /* PWM_H_ */
