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

/*========================================================================================*/
// Function: Update_TELEM
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Gather Telemetry Information for OBC. Voltage, current and temperature.
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
	telem[6] = Volt_ADC(0);
	telem[7] = Volt_ADC(0);
	telem[8] = Volt_ADC(0);
	telem[9] = Volt_ADC(0);
	telem[10] = Volt_ADC(0);
	telem[11] = Temp_ADC();
	
}