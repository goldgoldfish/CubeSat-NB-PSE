/*========================================================================================*/
//
// power_state_mat.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Ben Wedemire & Chris Thomas
// Date: 2019-03-17
// Description: Source file containing the default power state matrix and functions to
//				initialize the power matrix.
/*========================================================================================*/

#include "power_state_mat.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

void pwrMatInit(uint8_t* pwrMat){
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

/*========================================================================================*/
// Function: pwrUpdate
//
// Author: Kyle Andersen
// Date: 2019-03-20
// Description: 
/*========================================================================================*/

int PowerStateCheck(float SOC, float temp){
	if(SOC < 20){
		if(temp >= 40){
			return 4;
		}//end else if
		else if(temp >= 30 && temp < 40){
			return 3;
		}//end else if
		else if(temp >= 15 && temp < 30){
			return 2;
		}//end else if
		else if(temp >= 0 && temp < 15){
			return 1;
		}//end else if
		else{
			return 0;
		}//end else
	}//end if SOC < 20
	else if(SOC < 40 && SOC >= 20){
		if(temp >= 40){
			return 9;
		}//end else if
		else if(temp >= 30 && temp < 40){
			return 8;
		}//end else if
		else if(temp >= 15 && temp < 30){
			return 7;
		}//end else if
		else if(temp >= 0 && temp < 15){
			return 6;
		}//end else if
		else{
			return 5;
		}//end else
	}//end else if SOC < 40
	else if(SOC < 60 && SOC >= 40){
		if(temp >= 40){
			return 14;
		}//end else if
		else if(temp >= 30 && temp < 40){
			return 13;
		}//end else if
		else if(temp >= 15 && temp < 30){
			return 12;
		}//end else if
		else if(temp >= 0 && temp < 15){
			return 11;
		}//end else if
		else{
			return 10;
		}//end else
	}//end if SOC < 60
	else if(SOC < 80 && SOC <= 60){
		if(temp >= 40){
			return 19;
		}//end else if
		else if(temp >= 30 && temp < 40){
			return 18;
		}//end else if
		else if(temp >= 15 && temp < 30){
			return 17;
		}//end else if
		else if(temp >= 0 && temp < 15){
			return 16;
		}//end else if
		else{
			return 15;
		}//end else
	}//end if SOC < 80
	else {
		if(temp >= 40){
			return 24;
		}//end else if
		else if(temp >= 30 && temp < 40){
			return 23;
		}//end else if
		else if(temp >= 15 && temp < 30){
			return 22;
		}//end else if
		else if(temp >= 0 && temp < 15){
			return 21;
		}//end else if
		else{
			return 20;
		}//end else
	}//end else SOC > 80
}//end PowerStateCheck

/*========================================================================================*/
// Function: Update_STATE
//
// Author: Chris Thomas
// Date: 2019-03-25
// Description: Updates to the state to character array form for ease of use as it is
//				impossbile to read individual bits in a unsigned integer.
/*========================================================================================*/

void Update_STATE(char* power, uint8_t state){
	
	// String Index
	int i = 0;
	
	// Storage for subsystem states "ON" or "OFF"
	
	/* Data layout
	power[0] => VBatt   Heater
	power[1] => 5V      Heater
	power[2] => 8V      Heater
	power[3] => OBC     Power
	power[4] => Imager  Power
	power[5] => GRIPS   Power
	power[6] => SBand   Power
	power[7] => UHF     Power	
	*/
	
	// 8 bit mask to check the state of each position
	uint8_t mask = 0x01;
	// keep track of the first index
	
	while(i < 8){
		if (mask & state){
			power[i] = 'y';
			mask = mask << 1; // shift left once
		}
		else{
			power[i] = 'n';
			mask = mask << 1; // shift left once
		}
		i++;
	}
}

