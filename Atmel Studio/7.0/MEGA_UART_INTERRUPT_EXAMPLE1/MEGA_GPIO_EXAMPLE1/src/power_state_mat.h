/*========================================================================================*/
//
// power_state_mat.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Ben Wedemire
// Date: 2019-03-17
// Description: Source file containing the default power state matrix and functions to
// initialize the power matrix
/*========================================================================================*/

#include <stdint.h>

#ifndef POWER_STATE_MAT_H_
#define POWER_STATE_MAT_H_

// Initializes power state matrix to the default setting
void pwrMatInit(uint8_t* pwrMat);

// Checks the power state of the system based on Battery voltage and current readings
int PowerStateCheck(float SOC, float temp);

// Updates the power state to string form for ease of use
void Update_STATE(char* power, uint8_t state);

// Set Mode of the EPS (Auto/Manual)
void Set_MODE(uint8_t mode);

#endif /* power_state_mat_H_ */