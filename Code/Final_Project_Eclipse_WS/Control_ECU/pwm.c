/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the PWM driver
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "pwm.h"
#include "gpio.h"
#include "common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer0_init(void) {
    PWM_TIMER0_TCNT0 = 0; // Initialize the Timer0 counter

    /* Set OC0 pin as output pin */
    GPIO_setupPinDirection(PWM_OUTPUT_PIN, PWM_OUTPUT_PIN_NUMBER, PIN_OUTPUT);

    /* Configure Timer0 for PWM mode:
     * - Fast PWM Mode (WGM00, WGM01 = 1)
     * - Non-inverting mode (COM01 = 1, COM00 = 0)
     * - Set the prescaler to F_CPU/1024
     */
    PWM_TIMER0_TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS02) | (1 << CS00);
}

void PWM_Timer0_Start(uint8 duty_cycle) {
    /* Set the duty cycle: Calculate OCR0 value based on the input duty cycle */
    PWM_TIMER0_OCR0 = (uint8)(((uint16)duty_cycle * 255) / 100); // Corrected the parenthesis
}
