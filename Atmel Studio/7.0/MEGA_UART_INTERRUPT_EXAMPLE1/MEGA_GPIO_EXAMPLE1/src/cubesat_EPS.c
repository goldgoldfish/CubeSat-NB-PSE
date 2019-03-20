/*========================================================================================*/
//
// cubesat_EPS.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
// 
// Author: Chris Thomas
// Date: 2019-01-07
// Description: EPS microcontroller Software Routine.
/*========================================================================================*/

#include "compiler.h"
#include <board.h>
#include <avr/interrupt.h>
#include "conf_example.h"
#include "adc.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "wdt_megarf.h"
#include <avr/io.h>
#include <stdio.h>

// User Defined Libraries
#include "peripheral_init.h"
#include "adc_func.h"
#include "eeprom.h"
#include "uart_func.h"
#include "gpio_func.h"
#include "eeprom.h"
#include "obc_comm.h"

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Defining Parameters and Variables
/*========================================================================================*/


// POST Launch Bits
int p = 0;

// Telemetry information storage
float telem[12];
char* power[8];
int soc = 0;
char charge[5];

// Power State
uint8_t state = 0;


/*========================================================================================*/
// Function: Timer Interrupt
//
// Author: Chris Thomas
// Date: 2019-03-18
// Description: Interrupt occurs when timer register overflow flag is set, and
//				calls for telemetry data to be updated.
/*========================================================================================*/

ISR(TIMER1_COMPA_vect){
	
	// Disable Interrupts
	cli();
	
	Update_STATE(power,state);
	Update_TELEM(telem);
	Update_OBC(telem, power);
	soc = SoC_ADC(telem[10],telem[4]);
	if(soc >= 70){
		charge[0] = '7';
	}
	else{
		charge[0] = '0';
	}
	UART0_putchar(charge[0]);
	// Re-enable Interrupts
	sei();
		
}

int main(void){
	GPIO_init();
	UART0_init();
	ADC_init();
	CLK_init();
	
	sei();
	Set_GPIO(11);
	//Set_GPIO(3);
	_delay_ms(1000);
	Set_GPIO(10);
	Set_GPIO(28);
	_delay_ms(1000);
	Set_GPIO(16);
	Set_GPIO(26);
	
	while(true){
		
		
	}
	

}

