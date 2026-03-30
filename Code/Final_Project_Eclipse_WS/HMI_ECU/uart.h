/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver.
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/* Enum for Parity Types */
typedef enum
{
    UART_PARITY_DISABLED,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} UART_ParityType;

/* Enum for Number of Stop Bits */
typedef enum
{
    UART_ONE_STOP_BIT,
    UART_TWO_STOP_BITS
} UART_StopBitType;

/* Enum for Number of Data Bits */
typedef enum
{
    UART_5_BIT_DATA,
    UART_6_BIT_DATA,
    UART_7_BIT_DATA,
    UART_8_BIT_DATA,
    UART_9_BIT_DATA
} UART_DataBitType;

/* Enum for Baud Rate (Add more if needed) */
typedef uint32 UART_BaudRateType;

/* Enum for UART Mode (Polling or Interrupt for receiving) */
typedef enum
{
    UART_POLLING_MODE,  /* Use polling for receiving */
    UART_INTERRUPT_MODE /* Use interrupts for receiving */
} UART_ModeType;

/* Structure for UART Configuration */
typedef struct
{
    UART_BaudRateType baud_rate;
    UART_ParityType parity;
    UART_StopBitType stop_bit;
    UART_DataBitType data_bits;
    UART_ModeType mode; /* Mode for receiving (Polling or Interrupt) */
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initializes the UART with the specified configurations.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Sends a byte through UART using polling.
 */
void UART_sendByte(const uint16 data);

/*
 * Description :
 * Returns the received byte using polling if configured, or via interrupt if enabled.
 */
uint16 UART_receiveByte(void);

/*
 * Description :
 * Sends a string through UART using polling.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receives a string through UART until the '#' symbol, using polling or interrupts based on the configuration.
 */
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */
