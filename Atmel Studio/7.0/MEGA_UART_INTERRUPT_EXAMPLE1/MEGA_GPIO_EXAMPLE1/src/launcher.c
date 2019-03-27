/*========================================================================================*/
//
// launcher.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Source file containing all functions related to manipulating permanent
//				memory for pre-launch and post-launch states.
/*========================================================================================*/

#include "launcher.h"
#include "eeprom.h"
#include "wdt_megarf.h"
#include <util/delay.h>
#include <stdint.h>

/*========================================================================================*/
// Function: launch_state
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Checks the launch state, does nothing if launched. Starts the launch 
//				timer if not.
/*========================================================================================*/

void launch_state(){
	
	// Check if the satellite has been launched
	if(launch_check()){
		// don't set the timer
	}
	else{
		launch_timer();
		launch_post();
	}
	
}

/*========================================================================================*/
// Function: pre_launch
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Initializes the launch bits to zero (8 bits).
/*========================================================================================*/

void launch_pre(){
	EEPROM_Write(0x00,0x0000);
}

/*========================================================================================*/
// Function: post_launch
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Sets the launch bits to 1 (8 bits).
/*========================================================================================*/

void launch_post(){
	EEPROM_Write(0xff,0x0000);
}

/*========================================================================================*/
// Function: check_launch
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Checks the launch bits. If sum is 3 or greater, this means the CubeSat has
//				been launched.
/*========================================================================================*/

uint8_t launch_check(){
	
	// Read from permanent memory
	uint8_t launch = EEPROM_Read(0x0000);
		
	// Check mask
	uint8_t mask_check = 0x01;
	uint8_t sum = 0;
	int i = 0; // increment
	
	while(launch){
		launch = launch >> 1;
		sum += 1;
		if(sum > 3){
			return sum;
		}
	}
	return 0;
}

/*========================================================================================*/
// Function: reset_launch
//
// Author: Chris Thomas
// Date: 2019-03-26
// Description: Sets the launch bits to a pre-launch state and uses the watchdog to reset
//				the system.
/*========================================================================================*/

void launch_reset(){
	
	// Reset the launch bits to 0x00
	launch_pre();
	
	/* Watchdog must be reset to ensure the clock starts at 0.
	/ This is especially important as we are shortening the timeout period
	*/
	
	// Watchdog reset
	wdt_reset(); 
	
	// Set the watchdog to System Reset Mode
	wdt_enable(SYSTEM_RESET_MODE);
	
	// Change the tiemout period to 1.0s			 
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_128KCLK); // timeout set from 1024k to 128k
	
}

/*========================================================================================*/
// Function: timer_launch
//
// Author: Chris Thomas
// Date: 2019-03-26
// Description: Uses the util/delay.h library to create a delay simulating the delay
//				that will be needed once the nano satellite is in space.
/*========================================================================================*/

void launch_timer(){
	
	// Delay the start-up by 8 seconds
	_delay_ms(8000);
}