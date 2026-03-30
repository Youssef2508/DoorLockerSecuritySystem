/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer AVR driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "timer.h"
#include "avr/io.h" /* To use Timer Registers */
#include "common_macros.h" /* For bit manipulation macros */
#include <avr/interrupt.h> /* For Timer ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the addresses of the callback functions for each timer */
static volatile void (*g_timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2CallBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* ISR for Timer0 Overflow */
ISR(TIMER0_OVF_vect)
{
    if(g_timer0CallBackPtr != NULL_PTR)
    {
        (*g_timer0CallBackPtr)();
    }
}

/* ISR for Timer0 Compare Match */
ISR(TIMER0_COMP_vect)
{
    if(g_timer0CallBackPtr != NULL_PTR)
    {
        (*g_timer0CallBackPtr)();
    }
}

/* ISR for Timer1 Overflow */
ISR(TIMER1_OVF_vect)
{
    if(g_timer1CallBackPtr != NULL_PTR)
    {
        (*g_timer1CallBackPtr)();
    }
}

/* ISR for Timer1 Compare Match A */
ISR(TIMER1_COMPA_vect)
{
    if(g_timer1CallBackPtr != NULL_PTR)
    {
        (*g_timer1CallBackPtr)();
    }
}

/* ISR for Timer2 Overflow */
ISR(TIMER2_OVF_vect)
{
    if(g_timer2CallBackPtr != NULL_PTR)
    {
        (*g_timer2CallBackPtr)();
    }
}

/* ISR for Timer2 Compare Match */
ISR(TIMER2_COMP_vect)
{
    if(g_timer2CallBackPtr != NULL_PTR)
    {
        (*g_timer2CallBackPtr)();
    }
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType *Config_Ptr)
{
    switch(Config_Ptr->timer_ID)
    {
        case TIMER0_ID:
            TCNT0 = (uint8)(Config_Ptr->timer_InitialValue);  /* Set Timer0 initial value */

            if (Config_Ptr->timer_mode == TIMER_COMPARE_MODE)
            {
                OCR0 = (uint8)(Config_Ptr->timer_compare_MatchValue);  /* Set compare value */
                TCCR0 = (1 << WGM01);  /* CTC Mode */
                TIMSK |= (1 << OCIE0); /* Enable Timer0 Compare Match interrupt */
            }
            else
            {
                TCCR0 = 0x00; /* Normal Mode */
                TIMSK |= (1 << TOIE0); /* Enable Timer0 Overflow interrupt */
            }

            /* Set clock prescaler */
            TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock);
            break;

        case TIMER1_ID:
            TCNT1 = Config_Ptr->timer_InitialValue;  /* Set Timer1 initial value */

            if (Config_Ptr->timer_mode == TIMER_COMPARE_MODE)
            {
                OCR1A = Config_Ptr->timer_compare_MatchValue;  /* Set compare value */
                TCCR1B = (1 << WGM12);  /* CTC Mode */
                TIMSK |= (1 << OCIE1A); /* Enable Timer1 Compare Match A interrupt */
            }
            else
            {
                TCCR1B = 0x00; /* Normal Mode */
                TIMSK |= (1 << TOIE1); /* Enable Timer1 Overflow interrupt */
            }

            /* Set clock prescaler */
            TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock);
            break;

        case TIMER2_ID:
            TCNT2 = Config_Ptr->timer_InitialValue;  /* Set Timer2 initial value */

            if (Config_Ptr->timer_mode == TIMER_COMPARE_MODE)
            {
                OCR2 = Config_Ptr->timer_compare_MatchValue;  /* Set compare value */
                TCCR2 = (1 << WGM21);  /* CTC Mode */
                TIMSK |= (1 << OCIE2); /* Enable Timer2 Compare Match interrupt */
            }
            else
            {
                TCCR2 = 0x00; /* Normal Mode */
                TIMSK |= (1 << TOIE2); /* Enable Timer2 Overflow interrupt */
            }

            /* Set clock prescaler */
            TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock);
            break;
    }
}

void Timer_deInit(Timer_ID_Type timer_type)
{
    switch(timer_type)
    {
        case TIMER0_ID:
            TCCR0 = 0x00;   /* Disable Timer0 */
            TCNT0 = 0x00;   /* Clear Timer0 Counter */
            TIMSK &= ~(1 << TOIE0) & ~(1 << OCIE0);  /* Disable Timer0 interrupts */
            break;

        case TIMER1_ID:
            TCCR1B = 0x00;  /* Disable Timer1 */
            TCNT1 = 0x00;   /* Clear Timer1 Counter */
            TIMSK &= ~(1 << TOIE1) & ~(1 << OCIE1A); /* Disable Timer1 interrupts */
            break;

        case TIMER2_ID:
            TCCR2 = 0x00;   /* Disable Timer2 */
            TCNT2 = 0x00;   /* Clear Timer2 Counter */
            TIMSK &= ~(1 << TOIE2) & ~(1 << OCIE2);  /* Disable Timer2 interrupts */
            break;
    }
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    switch(a_timer_ID)
    {
        case TIMER0_ID:
            g_timer0CallBackPtr = a_ptr;
            break;
        case TIMER1_ID:
            g_timer1CallBackPtr = a_ptr;
            break;
        case TIMER2_ID:
            g_timer2CallBackPtr = a_ptr;
            break;
    }
}
