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

#include "power_state_mat.h"
#include <stdint.h>
#include <stdio.h>

/*========================================================================================*/
// Function: pwrMatInit
//
// Author: Ben Wedemire
// Date: 2019-03-17
// Description: This function will configure the power state matrix on power up by using
// the default one or pulling the one stored in EEPROM.
// The structure for the power state matrix is
//
/*========================================================================================*/

void pwrMatInit() {
	uint8_t pwrMat[25];
	pwrMat[0] = 0b00000001;
	pwrMat[1] = 0b00000001;
	pwrMat[2] = 0b00000000;
	pwrMat[3] = 0b00000000;
	pwrMat[4] = 0b00000000;
	pwrMat[5] = 0b00000000;
	pwrMat[6] = 0b00000100;
	pwrMat[7] = 0b00001010;
	pwrMat[8] = 0b00001000;
	pwrMat[9] = 0b00001000;
	pwrMat[10] = 0b00011100;
	pwrMat[11] = 0b00011010;
	pwrMat[12] = 0b00011000;
	pwrMat[13] = 0b00011000;
	pwrMat[14] = 0b00000000;
	pwrMat[15] = 0b10111100;
	pwrMat[16] = 0b00111010;
	pwrMat[17] = 0b00111000;
	pwrMat[18] = 0b00111000;
	pwrMat[19] = 0b00000000;
	pwrMat[20] = 0b11111100;
	pwrMat[21] = 0b11111010;
	pwrMat[22] = 0b11111000;
	pwrMat[23] = 0b11111000;
	pwrMat[24] = 0b00000000;
} //end pwrMatInit