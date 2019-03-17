/*========================================================================================*/
//
// adc_func.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-02-26
// Description: Header file containing all functions that utilize the ADC or change ADC
//				parameters.
/*========================================================================================*/

#include <stdint.h>

#ifndef ADC_FUNC_H_
#define ADC_FUNC_H_

// Change the ADC Channel
void Change_ADC(int i);

// Read from current ADC Channel
uint16_t Read_ADC();

// Read value from selected ADC Channel
float Check_ADC(int i);

// Voltmeter measurements
float Volt_ADC(int Volt_num);

// Ammeter measurements
float Amp_ADC(int Amp_num);

// Temperature measurement
float Temp_ADC();

#endif /* ADC_FUNC_H_ */