/*******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Youssef Hassan
 *
 *******************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/**
 * @brief Initializes the External EEPROM by setting up the TWI interface.
 *
 * @return uint8 Returns SUCCESS (1) if initialization is successful,
 *               otherwise returns ERROR (0).
 */
uint8 EEPROM_init(void);

/**
 * @brief Writes a byte to the specified address in the EEPROM.
 *
 * @param u16addr The address in the EEPROM where the byte will be written.
 * @param u8data The byte to be written to the EEPROM.
 *
 * @return uint8 Returns SUCCESS (1) if the write operation is successful,
 *               otherwise returns ERROR (0).
 */
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);

/**
 * @brief Reads a byte from the specified address in the EEPROM.
 *
 * @param u16addr The address in the EEPROM from which the byte will be read.
 * @param u8data A pointer to store the read byte.
 *
 * @return uint8 Returns SUCCESS (1) if the read operation is successful,
 *               otherwise returns ERROR (0).
 */
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data);

uint8 EEPROM_writeData(uint16 u16addr,uint8* u8data, uint8 size);

uint8 EEPROM_readData(uint16 u16addr,uint8 *u8data, uint8 size);

#endif /* EXTERNAL_EEPROM_H_ */
