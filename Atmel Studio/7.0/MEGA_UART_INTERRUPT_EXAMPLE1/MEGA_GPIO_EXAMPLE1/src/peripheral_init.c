/*========================================================================================*/
//
// peripherial_init.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-02-26
// Description: Source file containing all implementations corresponding to the initialization
//				functions found in "peripheral.h".
/*========================================================================================*/

// Initial Definitions
#define F_CPU 1000000u // CPU clock speed 
#define BAUD 4800 // Baud Rate for UART Transmission
#define BAUD_TOL 2 // % tolerance for UART Transmission


// ATmeag640 Board Libraries
#include "compiler.h"
#include <board.h>
#include <avr/interrupt.h>
#include "conf_example.h"
#include "adc.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "peripheral_init.h"
#include <util/setbaud.h>
#include <avr/iomxx0_1.h>
#include "wdt_megarf.h"

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: General Purpose I/O Port initialization
/*========================================================================================*/

void GPIO_init(){
	
	// Initializing the PORTs
	
	DDRH = 0x7F; // PORTH pins 0-6 set as outputs
	DDRL = 0x0E; // PORTL pins 1-3 set as outputs
	DDRA = 0xF0; // PORTA pins 4-7 set as outputs
	DDRJ = 0x7F; // PORTJ pins 0-6 set as outputs
	DDRC = 0xFF; // PORTC pins 0-7 set as outputs
	
	// Set Output as Low
	PORTH = 0x00;
	PORTL = 0x00;
	PORTA = 0x00;
	PORTJ = 0x00; 
	PORTC = 0x00;
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-22
// Description: Analog to Digital Converter initialization
/*========================================================================================*/

void ADC_init(){
	
	// Enable as Input
	DDRF = 0x00;
	
	// Enable the 1.1V internal reference
	//ADMUX |= ((REFS1 << 0) | (REFS0 << 0)); // set the first bit to 1
	ADMUX = 0x00;
	
	// Enable the ADC via the status control register & enable interrupt
	//ADCSRA |= (ADEN << 1) | (ADPS2 << 1) | (ADPS0 << 1) | (ADIE << 1);
	ADCSRA = 0x85;
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-07
// Description: Timer Initialization for Interrupt Routine
/*========================================================================================*/

CLK_init(){
	
	// Enable Timer0
	TCCR1B = 0x05;
	
	// Enable Output Compare Interrupt
	TIMSK1 = 0x02;
	
	// Set Counter to 128 clock cycles
	OCR1A = 0xffff;
	
	// Set clock to 0
	TCNT1 = 0x0000;
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-16
// Description: UART Initialization
/*========================================================================================*/

void UART0_init(){
	
	// Using the values from set baud tool
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	// Enable TX & RX and Enable RX complete interrupt
	UCSR0B = 0x18; // enabling bits 4 & 3 (RX & TX) ****** RE-ENABLE INTERRUPT!!!!!!!
	
	
	// Set up the format 8bit no parity
	UCSR0C = 0x06; // enable 8 bit chars
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-16
// Description: WatchDog Timer initialization.
/*========================================================================================*/

void WD_init(){
	
	// Set Prescaller
	
	// Watchdog Reset
	//wdt_enable(INTERRUPT_SYSTEM_RESET_MODE);

	// Watchdog timer enable
	//WDTCSR |= 0x10;
	// Watchdog Change Enable & Prescaler set (4s timer)
	//WDTCSR |= 0x30;
	
}