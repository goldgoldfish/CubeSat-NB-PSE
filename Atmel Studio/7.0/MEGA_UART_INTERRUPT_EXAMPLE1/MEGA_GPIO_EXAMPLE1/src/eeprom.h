/*========================================================================================*/
//
// eeprom.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Header file containing all functions related to EEPROM permanent
//				memory.
/*========================================================================================*/

#include <stdint.h>

#ifndef EEPROM_H_
#define EEPROM_H_

// Writing to EEPROM
void EEPROM_Write(uint8_t launch_bits, uint16_t address);

// Reading from EEPROM
uint8_t EEPROM_Read(uint16_t address);

#endif /* EEPROM_H_ */