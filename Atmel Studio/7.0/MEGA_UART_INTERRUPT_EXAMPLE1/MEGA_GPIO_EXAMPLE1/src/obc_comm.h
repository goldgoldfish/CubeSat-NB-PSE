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

#ifndef OBC_COMM_H_
#define OBC_COMM_H_

// Updates all telemetry information regarding voltage, current and temperature
void Update_TELEM(float* telem);

// Updates the power state based on the temperature and battery level
char* Update_STATE(uint8_t state);

#endif /* OBC_COMM_H_ */