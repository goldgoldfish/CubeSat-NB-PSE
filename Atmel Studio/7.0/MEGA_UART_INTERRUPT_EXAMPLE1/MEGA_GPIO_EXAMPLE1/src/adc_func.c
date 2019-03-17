/*========================================================================================*/
//
// adc_func.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-02-26
// Description: Source file containing all functions that utilize the ADC or change ADC
//				parameters.
/*========================================================================================*/

#include "adc_func.h"
#include "compiler.h"
#include <avr/interrupt.h>
#include "conf_example.h"
#include "adc.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: Change ADC_MUX to read next value
/*========================================================================================*/

void Change_ADC(int i){
	if (i < 4){
		ADMUX = 0x00 + i;
	}
	else{
		ADCSRB = 0x08;
		ADMUX = 0x00 + (i - 4);
	}
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-22
// Description: Read a value from the ADC data register
/*========================================================================================*/

uint16_t Read_ADC(){
	
	// start conversion
	ADCSRA |= 0x40;
	
	// Wait until conversion is done
	while(ADCSRA & (1 << ADSC));

	return(ADC);
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Checks the desired ADC Channel
/*========================================================================================*/

double Check_ADC(int i){
	
	// Choose the correct ADC channel to read from
	Change_ADC(i);
	
	// Get the read value from the ADC
	return Read_ADC();	
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Voltage Measurements via ADC.
/*========================================================================================*/

double Volt_ADC(int Volt_num){
	int ADC_num = Volt_num + 4; // ADC_num is the value needed for Change_ADC
	double val = 0;
	
	// Explained
	// voltage = (ADC_value / total resolution)
	
	// 3V3-1 & 3V3-2 rails
	if(ADC_num == 4 || ADC_num == 5){
		val = ((Check_ADC(ADC_num) / 1024) * 3.3) / 0.9090;
	}
	// 5V rail
	else if(ADC_num == 6){
		val = ((Check_ADC(ADC_num) / 1024) * 3.3) / 0.5997;
		// Correction factor (through testing)
		// 69 (mV) offset
	}
	// 8V rail
	else if(ADC_num == 7){
		val = ((Check_ADC(ADC_num) / 1024) * 3.3) / 0.3597;
	}
	// VBatt rail
	else if(ADC_num == 8){
		val = ((Check_ADC(ADC_num) / 1024) * 3.3) / 0.7141;
	}
	else{
		// do nothing
	}
	return val;
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Ammeter measurements via ADC.
/*========================================================================================*/

double Amp_ADC(int Amp_num){
	int ADC_num = 0; // initialize ADC number to check
	double val = 0; // initialize the voltage value to 0 V
	int i = 0;
	
	if(Amp_num < 4){
		ADC_num = Amp_num;
		while(i < 1000){
			val += (((Check_ADC(ADC_num) / 1024) * 3.3) - 0.494) / 0.4;
			i++;
		}
		val = val/1000;
	}
	else if(Amp_num == 4 || Amp_num == 5){
		ADC_num = Amp_num + 5;
		while(i < 1000){
			val += (((Check_ADC(ADC_num) / 1024) * 3.3) - 0.494) / 0.4;
			i++;
		}
		val = val/1000;
	}
	return val;
}