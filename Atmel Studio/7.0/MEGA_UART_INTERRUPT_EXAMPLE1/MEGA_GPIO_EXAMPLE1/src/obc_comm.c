/*========================================================================================*/
//
// obc_comm.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-17
// Description: Source file containing all functions related to OBC serial
//				communication with the EPS microcontroller.
/*========================================================================================*/

#define TOTAL_AMP 6
#define TOTAL_VOLT 5

#include "adc_func.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*========================================================================================*/
// Function: Update_TELEM
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates telemetry information for OBC. Voltage, current and temperature.
/*========================================================================================*/

void Update_TELEM(float* telem){
	
	// Ammeter measurements
	telem[0] = Amp_ADC(2); // 3V3-1 rail current
	telem[1] = Amp_ADC(3); // 3V3-2 rail current
	telem[2] = Amp_ADC(1); // 5V rail current
	telem[3] = Amp_ADC(0); // 8V rail current
	telem[4] = Amp_ADC(5); // VBatt rail current
	telem[5] = Amp_ADC(4); // Solar panel current
	
	// Voltmeter measurements
	telem[6] = Volt_ADC(0); // 3V3-1 rail voltage
	telem[7] = Volt_ADC(1); // 3V3-2 rail voltage
	telem[8] = Volt_ADC(2); // 5V rail voltage
	telem[9] = Volt_ADC(3); // 8V-1 rail voltage
	telem[10] = Volt_ADC(4); // VBatt rail voltage
	telem[11] = Temp_ADC(); // Battery temperature
	
}

/*========================================================================================*/
// Function: Update_STATE
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates the power state based on the temperature and battery level. This
//				is based on the power state matrix.
/*========================================================================================*/

**char Update_STATE(uint8_t state){
	
	// String Index
	int i = 0;
	
	// Storage for subsystem states "ON" or "OFF"
	char power[8][3];
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
	
	while(i < 8){
		if (mask & state){
			power[i] = "ON";
			i++;
			mask = mask << 1; // shift left once
		}
		else{
			power[i] = "OFF";
			i++;
			mask = mask << 1; // shift left once
		}
	}	
}