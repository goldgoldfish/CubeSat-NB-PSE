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
#include "fault.h"

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Defining Parameters and Variables
/*========================================================================================*/

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
uint8_t edit_num[1] = {0};

// Power State
uint8_t power_state[1] = {0};
uint8_t prev_state[1] = {0};
	
// Watchdog variable
uint8_t watchdog = 0;

// UART Interrupt stuff
char fake;

// Modes
uint8_t mode[1] = {1}; // assumes autonomous mode
uint8_t demo[1] = {0}; // assumes regular operation

// Start of demo variable
uint8_t demo_start = 1;

//uint8_t launch = 0x00;
//uint8_t check = 0x01;
//uint8_t sum_check = 0x00;

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
	
	if(mode[0] && !demo[0]){
		// Check state of charge
		soc = SoC_ADC(telem[10],telem[4]);
		// Update power state
		state_num = PowerStateCheck(soc, temp);
		power_state[0] = pwrMat[state_num];
		// Update the state of the loads
		Update_LOADS(power_state[0], power_state);
		Update_STATE(power, power_state[0]);
		Update_OBC(telem, power, state_num, mode[0], soc);
	}
	else if(demo[0] && mode[0]){
		if(demo_start){
			UART0_putstring("The demo has started.");
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			// Speed up the interrupt for the demo
			OCR1A = 0x3f;
			state_num = 22;
			soc = 90;
		}
		demo_start = 0; // demo initialization is done
		power_state[0] = pwrMat[state_num]; // update power state
		// Update the rest of the system to simulate the changes
		Update_LOADS(power_state[0], power_state);
		Update_STATE(power, power_state[0]);
		Update_OBC(telem, power, state_num, mode[0], soc);
		// Reset the demo variables
		if(soc < 20){
			// Return to regular interrupt speed
			OCR1A = 0xff;
			// Reset the demo variable
			demo_start = 1;
			// Return to regular operation
			demo[0] = 0;
			// Update the OBC that the demo is over
			UART0_putstring("The demo is complete.");
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
		}
		// Force the state of charge to change
		soc -= 20; // decrement the state of charge
		state_num -= 5; // decrement state number by 5 to switch loads
	}
	else{
		// Check state of charge
		soc = SoC_ADC(telem[10],telem[4]);
		state_num = 'M'; // stands for manual mode
		Update_LOADS(power_state[0], power_state);
		Update_STATE(power, power_state[0]);
		Update_OBC(telem, power, state_num, mode[0], soc);
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
	commandDecode(telem, mode, power_state, power, pwrMat, state_num, edit_num, demo, soc);
	
	//wdt_enable(INTERRUPT_SYSTEM_RESET_MODE);
	
	sei();
}

ISR(WDT_vect){
	cli();
	WD_init();
	sei();
}

int main(void){
	
	// Check the Watchdog right away
	watchdog = MCUSR & 0x08;

	// Now initialize all peripherals
	ALL_init();
	
	// Disable Interrupts upon start
	cli();
	
	// Check the reason for reset
	fault_check(watchdog);
	
	// Check State of Launch
	launch_state();	
	
	// Update power state matrix
	pwrMatInit(pwrMat);
	
	//****** RE-Enable ******//
	sei();
	
	while(true){
		
		
		
	}
	

}

