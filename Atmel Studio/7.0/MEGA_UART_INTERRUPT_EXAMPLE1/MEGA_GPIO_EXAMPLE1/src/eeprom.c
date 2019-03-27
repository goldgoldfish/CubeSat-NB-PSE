/*========================================================================================*/
//
// eeprom.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Source file containing all functions related to EEPROM, permanent
//				memory.
/*========================================================================================*/

#include "eeprom.h"
#include <board.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-03-05
// Description: Writes to permanent memory (EEPROM)
/*========================================================================================*/

void EEPROM_Write(uint8_t launch_bits, uint16_t address){
	
	// Wait for previous read/write cycle to be finished
	while(EECR & 0x02);
	
	// Select EEPROM Address (first address)
	EEAR = address; // first address
	
	// Write to data Register
	EEDR = launch_bits;
	
	// Master Write Enable
	EECR = 0x04;
	
	// Write Enable
	EECR = 0x02;

}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Reads from permanent memory (EEPROM)
/*========================================================================================*/

uint8_t EEPROM_Read(uint16_t address){
	
	// Wait for previous read/write cycle to be finished
	while(EECR & 0x02);
	
	// Select EEPROM Address (first address)
	EEAR = address;
	
	// Read Enable
	EECR = 0x01;
	
	return EEDR;
	
}

