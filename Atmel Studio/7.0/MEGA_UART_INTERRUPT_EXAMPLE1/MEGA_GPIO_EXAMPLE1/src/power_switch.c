/*========================================================================================*/
//
// power_switch.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Source file containing all functions related to switching power to
//				voltage rails and subsystems.
/*========================================================================================*/

#include "power_switch.h"
#include "gpio_func.h"
#include "adc_func.h"
#include <stdint.h>

/*========================================================================================*/
// Function: Enable_3V3_1
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Enables 3V3-1 rail power. Waits for transient to finish.
/*========================================================================================*/
void Enable_3V3_1(){

	// Set 5V rail for readings
	Set_GPIO(10);

	while(Volt_ADC(0) < 3.1){
		// Wait until 3V3-1 rail transient is finished
	}
}


/*========================================================================================*/
// Function: Enable_5V
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Enables 5V rail power. Waits for transient to finish.
/*========================================================================================*/
void Enable_5V(){

	// Set 5V rail for readings
	Set_GPIO(11);

	while(Volt_ADC(2) < 4.8){
		// Wait until 5V rail transient is finished
	}
}

/*========================================================================================*/
// Function: Enable_8V
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Enables 8V rail power. Waits for transient to finish.
/*========================================================================================*/
void Enable_8V(){

	// Set 8V rail for readings
	Set_GPIO(16);

	while(Volt_ADC(3) < 7.8){
		// Wait until 8V rail transient is finished
	}
}

/*========================================================================================*/
// Function: Enable_VBatt
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Enables VBatt rail power. Waits for transient to finish.
/*========================================================================================*/
void Enable_VBatt(){

	// Set 8V rail for readings
	Set_GPIO(16);

	while(Volt_ADC(4) < 2.5){
		// Wait until 8V rail transient is finished
	}
}

/*========================================================================================*/
// Function: Disable_3V3_1
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Disables 3V3-1 rail power. Checks if any subsystems are using the rail
//				before powering down.
/*========================================================================================*/

void Disable_3V3_1(uint8_t power_state){
	
	// 3V3-1 rail subsystem mask
	uint8_t mask_3V3_1 = 0b00100000;

	// Check for subsystems using the 3V3-1 rail
	if(power_state & mask_3V3_1){
		// Do not turn off the rail
	}
	else{
		Clear_GPIO(10); // turn 3V3-1 rail off if nothing is using the rail.
	}
}

/*========================================================================================*/
// Function: Disable_5V
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Disables 5V rail power. Checks if any subsystems are using the rail before
//				powering down.
/*========================================================================================*/

void Disable_5V(uint8_t power_state){
	
	// 5V rail subsystem mask
	uint8_t mask_5V = 0b00110010;
	
	// Check for subsystems using the 5V rail
	if(power_state & mask_5V){
		// do not turn the rail off
	}
	else{
		Clear_GPIO(11); // turn off the 5V rail if nothing is using the rail.
	}
}

/*========================================================================================*/
// Function: Disable_8V
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Disables 8V rail power. Checks if any subsystems are using the rail before
//				powering down.
/*========================================================================================*/

void Disable_8V(uint8_t power_state){
	
	// 5V rail subsystem mask
	uint8_t mask_8V = 0b11000100;
	
	// Check for subsystems using the 5V rail
	if(power_state & mask_8V){
		// do not turn the rail off
	}
	else{
		Clear_GPIO(16); // turn off the 5V rail if nothing is using the rail.
	}
}

/*========================================================================================*/
// Function: Disable_VBatt
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Disables 8V rail power. Checks if any subsystems are using the rail before
//				powering down. 
/*========================================================================================*/

void Disable_VBatt(uint8_t power_state){
	
	// 5V rail subsystem mask
	uint8_t mask_8V = 0b00000001;
	
	// Check for subsystems using the 5V rail
	if(power_state & mask_8V){
		// do not turn the rail off
	}
	else{
		Clear_GPIO(21); // turn off the 5V rail if nothing is using the rail.
	}
}

/*========================================================================================*/
// Function: Update_LOADS
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to subsystems based on the current power state. The
//				power state is treated the same way whether the EPS is in autonomous or
//				manual mode.
/*========================================================================================*/

void Update_LOADS(uint8_t power_state, uint8_t* power_pointer){
	
	// Heater switching
	heater_SWITCH(power_state, power_pointer);
	
	// OBC switching
	obc_SWITCH(power_state);
	
	// Imager switching
	imager_SWITCH(power_state);
	
	// GRIPS switching
	grips_SWITCH(power_state);
	
	// SBand switching
	sband_SWITCH(power_state);
	
	// UHF switching
	uhf_SWITCH(power_state);
	
}

/*========================================================================================*/
// Function: Heater_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to Heaters based on current power state. Only one heater
//				can be turned on at a time. If none or more than one heater is set to be
//				turned on, all heaters are turned off.
/*========================================================================================*/

void heater_SWITCH(uint8_t power_state, uint8_t* power_pointer){
	
	//*****NOTE*****//
	// Only one heater can be enabled at a time.
	// In the case that more than one heater is set to be enabled, this will be considered
	// an error and all heaters will be disabled.	
	
	// Heater masks
	uint8_t mask_heater_VBatt = 0b00000001;
	uint8_t mask_heater_5V = 0b00000010;
	uint8_t mask_heater_8V = 0b00000100;
	
	// Check which of the heaters is turned on
	
	// Vbatt Heater
	if((power_state & mask_heater_VBatt) && !(power_state & (mask_heater_5V | mask_heater_8V))){
		Disable_5V(power_state); // disable 5V rail if not needed
		Disable_8V(power_state); // disable 8V rail if not needed
		Clear_GPIO(3); // Turn off 5V heater
		Clear_GPIO(2); // Turn off 8V heater
		Enable_VBatt();
		Set_GPIO(0);  // Enable VBatt heater
	}
	
	// 5V Heater
	else if((power_state & mask_heater_5V) && !(power_state & (mask_heater_VBatt | mask_heater_8V))){
		Disable_VBatt(power_state); // disable VBatt rail if not needed
		Disable_8V(power_state); // disable 8V rail if not needed
		Clear_GPIO(0); // Turn off VBatt heater
		Clear_GPIO(2); // Turn off 8V heater
		Enable_5V();
		Set_GPIO(3); // Enable 5V heater
	}
	
	// 8V Heater
	else if((power_state & mask_heater_8V) && !(power_state & (mask_heater_VBatt | mask_heater_5V))){
		Disable_VBatt(power_state); // disable VBatt rail if not needed
		Disable_5V(power_state); // disable 5V rail if not needed
		Clear_GPIO(0); // Turn off VBatt heater
		Clear_GPIO(3); // Turn off 5V heater
		Enable_8V();
		Set_GPIO(2); // Enable 8V heater
	}
	else{
		// Turn off all heaters
		Clear_GPIO(0);
		Clear_GPIO(3);
		Clear_GPIO(2);
		
		// Turn off associated rails (rail logic is taken care of in the function)
		Disable_VBatt(power_state);
		Disable_5V(power_state);
		Disable_8V(power_state);
		
		// Change power state as they are all turned off now
		power_pointer[0] &= 0b11111000;
	}
}

/*========================================================================================*/
// Function: obc_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to OBC based on current power state.
/*========================================================================================*/

void obc_SWITCH(uint8_t power_state){
	
	// OBC mask
	uint8_t mask_OBC = 0b00001000;
	
	// Turn on OBC if it is set to one
	if(power_state & mask_OBC){
		Set_GPIO(24); // turn on OBC
	}
	else{
		Clear_GPIO(24); // turn off OBC
	}
}

/*========================================================================================*/
// Function: imager_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to Imager based on current power state.
/*========================================================================================*/

void imager_SWITCH(uint8_t power_state){
	
	// Imager Mask
	uint8_t mask_imager = 0b00010000;
	
	// Turn on Imager subcomponents if power state calls for it
	if(power_state & mask_imager){
		Enable_5V();
		Set_GPIO(23); // turn on imager beagle
		Set_GPIO(22); // turn on camera 
	}
	else{
		Clear_GPIO(22); // turn off camera
		Clear_GPIO(23); // turn off imager beagle
		Disable_5V(power_state); // turn off associated rail (5V)		
	}
}

/*========================================================================================*/
// Function: grips_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to GRIPS based on current power state.
/*========================================================================================*/

void grips_SWITCH(uint8_t power_state){
	
	// GRIPS mask
	uint8_t grips_mask = 0b00100000;
	
	// Turn on Grips subcomponents if power state calls for it
	if(power_state & grips_mask){
		Enable_5V();
		Enable_3V3_1();
		Set_GPIO(27); // turn on GNSS beagle
		Set_GPIO(28); // turn on OEM719
	}
	else{
		Clear_GPIO(28); // turn off OEM719
		Clear_GPIO(27); // turn off GNSS beagle
		Disable_3V3_1(power_state); // turn off 5V rail
		Disable_5V(power_state); // turn off 3V3-1 rail
	}
}

/*========================================================================================*/
// Function: sband_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to SBand antenna based on current power state.
/*========================================================================================*/

void sband_SWITCH(uint8_t power_state){
	
	// SBand mask
	uint8_t mask_sband = 0b01000000;
	
	// Turn on SBand components if power stated calls for it
	if(power_state & mask_sband){
		Enable_8V();
		Set_GPIO(26); // turn on SBand
	}
	else{
		Clear_GPIO(26); // turn off SBand
		Disable_8V(power_state); // turn off 8V rail
	}
}

/*========================================================================================*/
// Function: uhf_SWITCH
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Switches power to UHF based on current power state.
/*========================================================================================*/

uhf_SWITCH(uint8_t power_state){
	
	// UHF mask
	uint8_t mask_uhf = 0b10000000;
	
	// Turn on UHF components if power state calls for it
	if(power_state & mask_uhf){
		Enable_8V();
		Set_GPIO(25); // turn on UHF
	}
	else{
		Clear_GPIO(25); // turn off UHF
		Disable_8V(power_state); // turn off 8V rail
	}
}