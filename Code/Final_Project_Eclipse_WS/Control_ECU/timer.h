/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer AVR driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/* Enum for Timer ID Types */
typedef enum
{
    TIMER0_ID,
    TIMER1_ID,
    TIMER2_ID
} Timer_ID_Type;

/* Enum for Timer Clock Types */
typedef enum
{
    TIMER_NO_CLOCK,       /* No clock source (Timer/Counter stopped) */
    TIMER_F_CPU_CLOCK,    /* No prescaling */
    TIMER_F_CPU_8,        /* F_CPU/8 */
    TIMER_F_CPU_64,       /* F_CPU/64 */
    TIMER_F_CPU_256,      /* F_CPU/256 */
    TIMER_F_CPU_1024      /* F_CPU/1024 */
} Timer_ClockType;

/* Enum for Timer Modes */
typedef enum
{
    TIMER_NORMAL_MODE,        /* Timer counts up until overflow */
    TIMER_COMPARE_MODE = 2    /* Timer counts up until compare match */
} Timer_ModeType;

/* Structure for Timer Configuration */
typedef struct
{
    uint16 timer_InitialValue;
    uint16 timer_compare_MatchValue; /* Used in compare mode only */
    Timer_ID_Type timer_ID;
    Timer_ClockType timer_clock;
    Timer_ModeType timer_mode;
} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to initialize the Timer driver.
 * Inputs: Pointer to the configuration structure with type Timer_ConfigType.
 * Return: None.
 */
void Timer_init(const Timer_ConfigType *Config_Ptr);

/*
 * Description: Function to disable the Timer.
 * Inputs: Timer_ID.
 * Return: None.
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description: Function to set the Call Back function address to the required Timer.
 * Inputs: Pointer to Call Back function and Timer ID.
 * Return: None.
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
