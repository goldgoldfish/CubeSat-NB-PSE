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

#include "obc_comm.h"
#include "gpio_func.h"
#include "adc_func.h"
#include <string.h>
#include "uart_func.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

/*========================================================================================*/
// Function: command_decode
//
// Author: Ben Wedemire
// Date: 2019-03-17
// Description: Decodes commands from the OBC and calls their corresponding function.		
/*========================================================================================*/
void commandDecode (uint8_t* telem) {
	char* command = UART0_getstring();
	
	if (strlen(command) > 20) {
		UART0_putstring("Error! Command larger than 20 chars");
	} //end if
	else {
		if (strcmp(command, "TelemRqt ") == 0) {
			//Update_TELEM(telem);
			//send telem
		} //end if
		else if (strcmp(command, "SubSysRqt ") == 0) {	
			//process Subsystem request
		} //else if
		else if (strcmp(command, "PwrMatEdit ") == 0) {
			//edit the power state matrix
		} //end else if
		else if (strcmp(command, "PwrMatRqt ") == 0) {
			//send the power state matrix cell to the obc
		} //end else if
		else if (strcmp(command, "ModeChange ") == 0) {
			//change or update the mode of the micro
		} //end else if
		else if (strcmp(command, "LaunchReset ") == 0) {
			//reset the system to a prelaunch config
		} //end else if
		else if (strcmp(command, "HelloCheck ") == 0) {
			//say hello
		} //end else if
		else {
			UART0_putstring(strcat("Error! The following command isn't recognized: %s", command));
		} //end else
	} //end else
} //end commandDecode

/*========================================================================================*/
// Function: Update_TELEM
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates telemetry information for OBC. Voltage, current and temperature.
/*========================================================================================*/

void Update_TELEM(float* telem){
	
	Set_GPIO(11);
	
	while(Volt_ADC(2) < 4.8){
		// Wait until 5V rail transient is finished
	}
	
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
			power[i] = 'Y';
			mask = mask << 1; // shift left once
		}
		else{
			power[i] = 'N';
			mask = mask << 1; // shift left once
		}
		i++;
	}
}

/*========================================================================================*/
// Function: subSysRqt
//
// Author: Ben Wedemire
// Date: 2019-03-17
// Description: Determines if a sub System can be powered up
/*========================================================================================*/

void subSysRqt () {
	
	//check if we are in manual mode
	
	//if we are service the request
	//
	
} //end subSysRqt

/*========================================================================================*/
// Function: Update_OBC
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates the telemetry information to the OBC.
/*========================================================================================*/

void Update_OBC(float* telem, char* power){
	// Prints a border to the Putty
	char stars[60] = "/***************************************CubeSat EPS"; // left side of border
	char stars2[45] = "***************************************/"; // right side of border
	char* border; // complete border top and bottom
	border = strcat(stars,stars2);
	
	// Print border
	UART0_putstring(border);
	
	// Spacing
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	
	// String Initializations
	char loads[8][10] = {"Heater 1", "Heater 2", "Heater 3", "OBC", "Imager", "GRIPS", "S-Band", "UHF"};
	char sensors[12][10] = {"3V3-1 [A]", "3V3-2 [A]", "5V [A]", "8V [A]", "VBatt [A]", "Solar [A]",
							"3V3-1 [V]", "3V3-2 [V]", "5V [V]", "8V [V]", "VBatt [V]", "Temp [C]"};
							
	// Doing to the spacing correctly						
	int spacing = 15;
	int spaces = 0;
	int spaces2 = 0; // for inner loops
	int l_incr = 0; // load increment (8)
	int s_incr = 0; // sensor increment (12)
	int i; // increment for telemetry (0 to 6, then 6 to 12)
	char measure[10]; // used for sprintf
	
	// Print the telemetry information
	while (s_incr < 12){
		
		// Switch lines after 6 titles are written and write
		// data on the next line
		if (s_incr == 6){
			UART0_putchar('\n');
			UART0_putchar('\r');
			// print values
			while(i < s_incr){
				while(spaces2){
					UART0_putchar(' ');
					spaces2--;
				}
				sprintf(measure,"%f",telem[i]);
				UART0_putstring(measure);
				i++;
				spaces2 = spacing - strlen(measure);
			}
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			spaces = 0; // reset for next line of titles
		}
		
		// Put the titles in normally
		// re-initialize the spaces
		while(spaces){
			UART0_putchar(' ');
			spaces--;
		}
		UART0_putstring(sensors[s_incr]);
		s_incr++;
		spaces = spacing - strlen(sensors[s_incr - 1]);
		
		// Print the second round of telemetry data
		if (s_incr == 12){
			spaces2 = 0; // re-initialize spaces to 0
			UART0_putchar('\n');
			UART0_putchar('\r');
			// print values
			while(i < s_incr){
				while(spaces2){
					UART0_putchar(' ');
					spaces2--;
				}
				sprintf(measure,"%f",telem[i]);
				UART0_putstring(measure);
				i++;
				spaces2 = spacing - strlen(measure);
			}
		}
		
	}
	
	
	// Print the Load Titles
	spaces = 0; // re initialize spaces
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	while(l_incr < 8){
		while(spaces){
			UART0_putchar(' ');
			spaces--;
		}
		UART0_putstring(loads[l_incr]);
		l_incr++;
		spaces = spacing - strlen(loads[l_incr - 1]);
	}
	
	// Print the load states
	// Print the Load Titles
	l_incr = 0; // reset load increment to 0
	spaces = 0; // re initialize spaces
	UART0_putchar('\n');
	UART0_putchar('\r');
	while(l_incr < 8){
		while(spaces){
			UART0_putchar(' ');
			spaces--;
		}
		UART0_putchar(power[l_incr]);
		l_incr++;
		spaces = spacing - 1;
	}
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
}

