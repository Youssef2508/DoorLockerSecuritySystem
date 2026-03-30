/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use macros like SET_BIT */
#include <avr/interrupt.h> /* For UART ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variable to hold the received byte from the ISR */
static volatile uint16 g_receivedData = 0;

/* Global variable to store the UART mode (Polling or Interrupt) */
static UART_ModeType g_uartMode;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(USART_RXC_vect)
{
    /* Store the received data in the global variable */
    g_receivedData = UDR;
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void UART_init(const UART_ConfigType *Config_Ptr)
{
    uint16 ubrr_value = 0;

    /* Disable U2X for normal transmission speed */
    UCSRA = 0x00;

    /* Configure frame format */
    uint8 UCSRC_temp = 0x80; /* URSEL = 1 to write to UCSRC */

    /* Set parity mode */
    switch (Config_Ptr->parity)
    {
    case UART_PARITY_DISABLED:
        /* No parity bits */
        break;
    case UART_PARITY_EVEN:
        UCSRC_temp |= (1 << UPM1);
        break;
    case UART_PARITY_ODD:
        UCSRC_temp |= (1 << UPM1) | (1 << UPM0);
        break;
    }

    /* Set stop bits */
    if (Config_Ptr->stop_bit == UART_TWO_STOP_BITS)
    {
        UCSRC_temp |= (1 << USBS);
    }

    /* Set data bits */
    switch (Config_Ptr->data_bits)
    {
    case UART_5_BIT_DATA:
        /* UCSZ1:0 = 00 */
        break;
    case UART_6_BIT_DATA:
        UCSRC_temp |= (1 << UCSZ0);
        break;
    case UART_7_BIT_DATA:
        UCSRC_temp |= (1 << UCSZ1);
        break;
    case UART_8_BIT_DATA:
        UCSRC_temp |= (1 << UCSZ1) | (1 << UCSZ0);
        break;
    case UART_9_BIT_DATA:
        UCSRC_temp |= (1 << UCSZ1) | (1 << UCSZ0);
        UCSRB |= (1 << UCSZ2);
        break;
    }

    UCSRC = UCSRC_temp;

    /* Set UART Mode (Polling or Interrupt) */
    g_uartMode = Config_Ptr->mode;

    /* Enable UART Receiver and Transmitter */
    UCSRB = (1 << RXEN) | (1 << TXEN);

    /* If Interrupt mode is selected, enable RX Complete Interrupt */
    if (g_uartMode == UART_INTERRUPT_MODE)
    {
        UCSRB |= (1 << RXCIE);
    }

    /* Calculate UBRR value for baud rate */
    ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 16UL))) - 1);

    /* Set baud rate */
    UBRRH = (ubrr_value >> 8) & 0x0F;
    UBRRL = ubrr_value;
}

void UART_sendByte(const uint16 data)
{
    /* Wait for the transmit buffer to be empty */
    while (BIT_IS_CLEAR(UCSRA, UDRE));

    /* Check for 9-bit data */
    if (BIT_IS_SET(UCSRB, UCSZ2))
    {
        UCSRB = (UCSRB & 0xFE) | ((data >> 8) & 0x01);
    }

    /* Put data into buffer, sends the data */
    UDR = data;
}

uint16 UART_receiveByte(void)
{
    if (g_uartMode == UART_POLLING_MODE)
    {
        /* Use polling: wait for data to be received */
        while (BIT_IS_CLEAR(UCSRA, RXC));
        return UDR; /* Return the received data */
    }
    else if (g_uartMode == UART_INTERRUPT_MODE)
    {
        /* Interrupt mode: return data captured by ISR */
        return g_receivedData;
    }
    return 0;
}

void UART_sendString(const uint8 *Str)
{
    uint8 i = 0;

    while (Str[i] != '\0')
    {
        UART_sendByte(Str[i]);
        i++;
    }
}

void UART_receiveString(uint8 *Str)
{
    uint8 i = 0;
    Str[i] = UART_receiveByte(); /* Receive first character */

    /* Keep receiving until the stop symbol '#' */
    while (Str[i] != '#')
    {
        i++;
        Str[i] = UART_receiveByte();
    }

    /* Null-terminate the string */
    Str[i] = '\0';
}
