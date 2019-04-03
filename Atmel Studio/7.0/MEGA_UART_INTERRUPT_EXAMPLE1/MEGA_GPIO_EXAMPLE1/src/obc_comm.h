/*========================================================================================*/
//
// obc_comm.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-17
// Description: Header file containing all functions related to OBC serial
//				communication with the EPS microcontroller.
/*========================================================================================*/

#include <stdint.h>

#ifndef OBC_COMM_H_
#define OBC_COMM_H_

// Takes commands from OBC and calls the corresponding function
void commandDecode (double* telem, uint8_t* mode, uint8_t* power_state,
					char* power, uint8_t* pwrMat, uint8_t state_num, 
					uint8_t* edit_num, uint8_t* demo, int soc);

// Updates all telemetry information regarding voltage, current and temperature
void Update_TELEM(double* telem, uint8_t power_state);

// Updates the OBC will all telemetry data stored on microcontroller
void Update_OBC(double* telem, char* power, uint8_t state_num, uint8_t mode, int soc);

// Manual Reset of the EPS Microcontroller (OBC controlled)
void Manual_RESET();

#endif /* OBC_COMM_H_ */