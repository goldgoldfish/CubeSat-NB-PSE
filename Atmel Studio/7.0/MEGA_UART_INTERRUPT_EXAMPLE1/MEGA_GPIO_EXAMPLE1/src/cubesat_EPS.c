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
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <stdio.h>
#include <float.h>
#include "wdt_megarf.h"

// User Defined Libraries
#include "peripheral_init.h"
#include "adc_func.h"
#include "eeprom.h"
#include "uart_func.h"
#include "gpio_func.h"
#include "eeprom.h"
#include "obc_comm.h"
#include "power_state_mat.h"
#include "power_switch.h"
#include "launcher.h"

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Defining Parameters and Variables
/*========================================================================================*/


// POST Launch Bits

// Telemetry information storage
double telem[12];
char power[9];

// stat of charge
float soc = 0;
float prev_soc = 0;

// temperature initialization
float temp = 0;
float prev_temp = 0;

// Power State matrix
uint8_t pwrMat[25];
int state_num = 0;

// Power State
uint8_t power_state[1] = {0};
uint8_t prev_state[1] = {0};

// Demo variables
char obc_command[20]; // <----- **** Get rid of this soon *****///
char fake;
uint8_t mode[1] = {1}; // assumes manual mode

uint8_t launch = 0x00;
uint8_t check = 0x01;
uint8_t sum_check = 0x00;

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
	
	// Update Telemetry information
	Update_TELEM(telem, power_state[0]);
	
	// Check Power State
	temp = telem[11];
	soc = SoC_ADC(telem[10],telem[4]);
	
	if(mode[0]){
		// Update power state
		state_num = PowerStateCheck(soc, temp);
		power_state[0] = pwrMat[state_num];
		// Update the state of the loads
		Update_STATE(power, power_state[0]);
		Update_LOADS(power_state[0], power_state);
		Update_OBC(telem, power);
	}
	else{
		Update_STATE(power, power_state[0]);
		Update_LOADS(power_state[0], power_state);
		Update_OBC(telem, power);
	}

	// Update OBC with newly acquired info
	
	// Print the state of charge and power matrix index
	
	
	// Save previous states for less computation
	prev_state[0] = power_state[0];
	prev_temp  = temp;
	prev_soc = soc;
	
	// Re-enable interrupts
	sei();
		
}

ISR(USART0_RX_vect){
	
	cli();
	
	//wdt_disable();
	
	fake = UART0_getchar();
	commandDecode(obc_command, telem, mode, power_state, power);
	
	//wdt_enable(INTERRUPT_SYSTEM_RESET_MODE);
	
	sei();
}

ISR(WDT_vect){
	cli();
	WD_init();
	sei();
}

int main(void){
	
	ALL_init();
	
	// Disable Interrupts upon start
	cli();
	
	UART0_putstring("I have been reset");
	
	launch_state();
	
	// Update power state matrix
	pwrMatInit(pwrMat);
	
	//****** RE-Enable ******//
	sei();
	
	while(true){
		
		
		
	}
	

}

