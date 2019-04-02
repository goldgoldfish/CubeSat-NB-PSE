/*========================================================================================*/
//
// fault.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-04-01
// Description: Header file containing all functions related to fault detection and fault
//				handling within the EPS.
/*========================================================================================*/

#include <stdint.h>

#ifndef FAULT_H_
#define FAULT_H_

// Checks and displays the cause of reset
void fault_check(uint8_t watchdog);

#endif /* FAULT_H_ */