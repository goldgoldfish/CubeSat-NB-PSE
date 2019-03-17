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
#include "uart_func.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <float.h>

/*========================================================================================*/
// Function: Change_ADC
//
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
// Function: Read_ADC
//
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
// Function: Check_ADC
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Checks the desired ADC Channel
/*========================================================================================*/

float Check_ADC(int i){
	
	// Choose the correct ADC channel to read from
	Change_ADC(i);
	
	// Get the read value from the ADC
	return Read_ADC();	
	
}

/*========================================================================================*/
// Function: Volt_ADC
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Voltage Measurements via ADC.
/*========================================================================================*/

float Volt_ADC(int Volt_num){
	int ADC_num = Volt_num + 4; // ADC_num is the value needed for Change_ADC
	float val = 0;
	
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
// Function: Amp_ADC
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Ammeter measurements via ADC.
/*========================================================================================*/

float Amp_ADC(int Amp_num){
	int ADC_num = 0; // initialize ADC number to check
	float val = 0; // initialize the voltage value to 0 V
	int i = 0;
	
	// Voltage rail current 8V, 5V, 3V3-1, 3V3-2 respectively
	if(Amp_num < 4){
		ADC_num = Amp_num;
		while(i < 1000){
			val += (((Check_ADC(ADC_num) / 1024) * 3.3) - 0.494) / 0.4;
			i++;
		}
		val = val/1000;
	}
	// Solar current and Battery current, respectively
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

/*========================================================================================*/
// Function: Temp_ADC
//
// Author: Chris Thomas
// Date: 2019-03-17
// Description: Temperature measurement via ADC.
/*========================================================================================*/

float Temp_ADC(){
	
}

/*========================================================================================*/
// Function: SoC_ADC
//
// Author: Chris Thomas
// Date: 2019-03-17
// Description: Temperature measurement via ADC.
/*========================================================================================*/

int SoC_ADC(){
	// return value
	int soc = 0;
	
	// Check Battery voltage and current
	float amp = Amp_ADC(5);
	float volt = Volt_ADC(4);
	
	// Lookup table
	if (amp < 0.6 && amp > 0){
		if (volt > 4.00) {
			soc = 80;
		} //end if
		else if (volt < 4.00 && volt > 3.80) {
			soc = 70;
		} //end else if
		else if (volt < 3.80 && volt > 3.75) {
			soc = 60;
		} //end else if
		else if (volt < 3.75 && volt > 3.70) {
			soc = 50;
		} //end else if
		else if (volt < 3.70 && volt > 3.60) {
			soc = 40;
		} //end else if
		else if (volt < 3.60 && volt > 3.5) {
			soc = 30;
		} //end else if
		else if (volt < 3.50) {
			soc = 20;
		} //end else if
		else {
			UART0_putstring("Error Reading State of Charge \n\n\r");
		} //end else
	}
	else if(amp > 0.6 && amp < 3){
		if (volt > 3.75) {
			soc = 80;
		} //end if
		else if (volt < 3.75 && volt > 3.65) {
			soc = 70;
		} //end else if
		else if (volt < 3.65 && volt > 3.55) {
			soc = 60;
		} //end else if
		else if (volt < 3.55 && volt > 3.45) {
			soc = 50;
		} //end else if
 		else if (volt < 3.45 && volt > 3.35) {
			soc = 40;
		 } //end else if
		else if (volt < 3.35 && volt > 3.30) {
			soc = 30;
		 } //end else if
		else if (volt < 3.30) {
			soc = 20;
		}
		else {
			UART0_putstring("Error Reading State of Charge \n\n\r");
		} //end else
	}
	else if(amp > 3){
		if (volt > 3.80) {
			soc = 80;
		} //end if
		else if (volt < 3.80 && volt > 3.75) {
			soc = 70;
		} //end else if
		else if (volt < 3.75 && volt > 3.65) {
			soc = 60;
		} //end else if
		else if (volt < 3.65 && volt > 3.50) {
			soc = 50;
		} //end else if
		else if (volt < 3.50 && volt > 3.40) {
			soc = 40;
		} //end else if
		else if (volt < 3.40 && volt > 3.25) {
			soc = 30;
		} //end else if
		else if (volt < 3.25) {
			soc = 20;
		} //end else if
		else {
			UART0_putstring("Error Reading State of Charge \n\n\r");
		} //end else
	}
	else{
		UART0_putstring("Battery Charging");
	}
}