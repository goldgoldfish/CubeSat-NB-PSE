/*========================================================================================*/
//
// power_switch.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-24
// Description: Header file containing all functions related to switching power to
//				voltage rails and subsystems.
/*========================================================================================*/

#include <stdint.h>

#ifndef INCFILE1_H_
#define INCFILE1_H_

// Switches all loads based on power state
void Update_LOADS(uint8_t power_state, uint8_t* power_pointer);

//*****NOTE*****//
// The 3V3-2 rail powers the microcontroller indefinitely and cannot be switched

// Safely enables the 3V3-1 rail
void Enable_3V3_1();

// Safely enables the 5V rail
void Enable_5V();

// Safely enables the 8V rail
void Enable_8V();

// Safely enables the VBatt rail
void Enable_VBatt();

// Safely disables 3V3-1 rail
void Disable_3V3_1(uint8_t power_state);

// Safely disables 5V rail
void Disable_5V(uint8_t power_state);

// Safely disables 8V rail
void Disable_8V(uint8_t power_state);

// Safely disables VBatt rail
void Disable_VBatt(uint8_t power_state);

// Switches power to the heaters (only one can be enabled at a time)
void heater_SWITCH(uint8_t power_state, uint8_t* power_pointer);

// Switches power to OBC
void obc_SWITCH(uint8_t power_state);

// Switches power to Imager
void imager_SWITCH(uint8_t power_state);

//Switches power to GRIPS
void grips_SWITCH(uint8_t power_state);

// Switches power to SBand
void sband_SWITCH(uint8_t power_state);

// Switches power to UHF
void uhf_SWITCH(uint8_t power_state);

#endif /* INCFILE1_H_ */