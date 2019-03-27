/*========================================================================================*/
//
// launcher.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Header file containing all functions related to manipulating permanent
//				memory for pre-launch and post-launch states.
/*========================================================================================*/

#include <stdint.h>

#ifndef LAUNCHER_H_
#define LAUNCHER_H_

// Initial Launch Check
void launch_state();

// Pre-Launch permanent memory initialization
void launch_pre();

// Post-Launch writing to permanent memory
void launch_post();

// Checks the state of the launch bits and returns either 0 or a value
uint8_t launch_check();

// Resets the microcontroller to a pre-launch state
void launch_reset();

// Launch timer (30 minutes in space, much less for demo purposes)
void launch_timer();

#endif /* LAUNCHER_H_ */