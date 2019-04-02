/*========================================================================================*/
//
// fault.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-04-01
// Description: Source file containing all functions related to fault detection and fault
//				handling within the EPS.
/*========================================================================================*/

#include "fault.h"
#include "eeprom.h"
#include <board.h>
#include "uart_func.h"
#include <stdint.h>

void fault_check(uint8_t watchdog){
	
	// Check the Watchdog reset bit
	if(watchdog && EEPROM_Read(0x0001)){
		EEPROM_Write(0x00, 0x0001); // Reset the bits to 0
		UART0_putstring("A manual reset has occurred.");
		UART0_putchar('\r');
		UART0_putchar('\n');
		UART0_putchar('\n');
	}
	else if(watchdog && !EEPROM_Read(0x0000)){
		UART0_putstring("A launch reset occurred.");
		UART0_putchar('\r');
		UART0_putchar('\n');
		UART0_putchar('\n');
	}
	// check the jtag reset bit
	else if(watchdog){
		UART0_putchar('\n');
		UART0_putchar('\r');
		UART0_putstring("A Fault occurred.");
		UART0_putchar('\r');
		UART0_putchar('\n');
		UART0_putchar('\n');
	}
	else if(MCUSR & 0x10){
		UART0_putstring("I have been reprogrammed.");
		UART0_putchar('\r');
		UART0_putchar('\n');
		UART0_putchar('\n');
	}
	// assume a power reset if none of the other flags are found
	else{
		UART0_putstring("A power reset has occurred.");
		UART0_putchar('\r');
		UART0_putchar('\n');
		UART0_putchar('\n');
	}
}