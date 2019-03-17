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


/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Libraries
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

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Defining Parameters and Variables
/*========================================================================================*/

#define MAX_ADC 4

// POST Launch Bits
int p = 0;

// Telemetry information storage
uint16_t telem[16];

// Power State Matrix


/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Gather Telemetry Information for OBC
/*========================================================================================*/

uint16_t* Check_TELEM(){
	
	int i = 0;
	while (i < MAX_ADC) {
		if (i > 0){
			Change_ADC(i);
			telem[i] = Read_ADC();
			i++;
		}
		else{
			ADMUX = 0x00;
			telem[i] = Read_ADC();
			i++;
		}	
	}
	return telem;
}

ISR(TIMER1_COMPA_vect){
	
	Check_TELEM();
		
}

int main(void)
{

// Initializing the GPIO
GPIO_init();
WD_init();
ADC_init();
UART0_init();
//CLK_init();
//sei();
//wdt_enable(INTERRUPT_SYSTEM_RESET_MODE);

	double current = 0;
	int i = 3;
	int j = 0;
	char measure[10];
	char* string;
	//while (true){
		
		//current = Amp_ADC(i);
		//sprintf(measure,"%f",current);
		//UART0_putstring(measure);
		string = UART0_getstring();
		UART0_putstring(string);
		//i++;
	//}
}
