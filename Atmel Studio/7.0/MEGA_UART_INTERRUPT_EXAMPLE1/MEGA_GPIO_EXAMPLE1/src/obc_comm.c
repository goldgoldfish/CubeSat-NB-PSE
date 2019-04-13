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
#include "power_switch.h"
#include "power_state_mat.h"
#include "launcher.h"
#include "gpio_func.h"
#include "adc_func.h"
#include <string.h>
#include "uart_func.h"
#include "compiler.h"
#include "wdt_megarf.h"
#include "eeprom.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

/*========================================================================================*/
// Function: command_decode
//
// Author: Ben Wedemire & Chris Thomas
// Date: 2019-03-17
// Description: Decodes commands from the OBC and calls their corresponding function.		
/*========================================================================================*/
void commandDecode (double* telem, uint8_t* mode, uint8_t* power_state,
					char* power, uint8_t* pwrMat, uint8_t state_num, 
					uint8_t* edit_num, uint8_t* demo, int soc){
	
	// Erase the first echoed letter
	UART0_putchar('\b'); // ascii code for backspace
	
	// List of OBC Commands
	// OBC Commands
	char obc_command[9][20] = {"TelemRqt", "SubSysRqt",
							   "PwrMatEdit", "ModeChange",
							   "LaunchReset", "HelloCheck", "Reset", "Demo"};
							   
	// Print list of commands for the user
	int i = 0; // increment
	
	// Printing the list cleanly
	UART0_putstring("List of available OBC Commands: ");
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	// print all seven commands
	while(i < 8){
		UART0_putchar(0x31 + i); // the '1' plus the command index
		UART0_putstring(" -> ");
		UART0_putstring(obc_command[i]);
		UART0_putchar('\n');
		UART0_putchar('\r');
		i++;
	}
	
	// Prompt the User
	UART0_putchar('\n');
	UART0_putstring("Enter the desired command:");
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	UART0_putstring("> ");		
	
	// Prompt user to try again if incorrect entry
	int incorrect = 1;
	
	// Check for command in list, if not in list, prompt user again
	
	while(incorrect){
		// Receive command index from OBC
		int com_ind = UART0_getchar() - 0x31; // negate the '0' char value to get the index
		UART0_putchar('\n');
		UART0_putchar('\n');
		UART0_putchar('\r');
		
		// Print the selected Command
		UART0_putstring("You have Selected: ");
		UART0_putstring(obc_command[com_ind]);
		UART0_putchar('\n');
		UART0_putchar('\n');
		UART0_putchar('\r');
			
		incorrect = 0; // assume the user inputs the correct command until it isn't found
			
		if (strcmp(obc_command[com_ind], "TelemRqt") == 0) {
			UART0_putstring("Updating Telemetry Information");
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			Update_TELEM(telem, power_state);
			Update_OBC(telem, power, state_num, mode[0], soc);
			//send telem
		} //end if
		else if (strcmp(obc_command[com_ind], "SubSysRqt") == 0) {	
			if(!mode[0]){
				// power_state = some new value (input yet to be determined, handle in GUI)
				UART0_putstring("Enter the desired state: ");
				UART0_putchar('\n');
				UART0_putchar('\n');
				UART0_putchar('\r');
				UART0_putstring("> ");	
				UART0_getstring(power);
				Manual_STATE(power, power_state);
				//UART0_putstring(power);  *****---->>> Putty echo now enabled not need
				Update_LOADS(power_state[0], power_state);
				UART0_putchar('\n');
				UART0_putchar('\n');
				UART0_putchar('\r');
			}
			else{
				UART0_putstring("I am currently in Autonomous mode.");
				UART0_putchar('\n');
				UART0_putchar('\n');
				UART0_putchar('\r');
				UART0_putstring("Select ModeChange [4] before attempting to change the power state.");
				UART0_putchar('\n');
				UART0_putchar('\n');
				UART0_putchar('\r');
				// Tell the user the EPS is still in autonomous mode and it must be changed
			}
		} //else if
		else if (strcmp(obc_command[com_ind], "PwrMatEdit") == 0) {
			pwrMatEdit(edit_num, power, pwrMat);
		} //end else if
		else if (strcmp(obc_command[com_ind], "ModeChange") == 0) {
			// The character 0 must be sent to go into manual mode
			mode[0] = !mode[0]; // toggle the current mode
			if(!mode[0]){
				UART0_putstring("I am now in Manual mode.");
			}
			else{
				UART0_putstring("I am now in Autonomous mode.");
			}
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			//change or update the mode of the micro
			// In this state do not update the power state, simply remain in the one specified
			// Have an SOC thing here
		} //end else if
		else if (strcmp(obc_command[com_ind], "LaunchReset") == 0) {
			launch_reset();
			//reset the system to a pre-launch config
			// Use the watchdog to reset the system
		} //end else if
		else if (strcmp(obc_command[com_ind], "HelloCheck") == 0) {
			UART0_putstring("Oh Hello, I am CubeSat EPS. What's your Zodiac sign?");
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
		} //end else if
		else if(strcmp(obc_command[com_ind], "Reset") == 0){
			Manual_RESET(); // Reset the EPS without changing launch state
		}
		else if(strcmp(obc_command[com_ind], "Demo") == 0){
			demo[0] = !demo[0]; // toggle demo mode
		}
		else {
			incorrect = 1;
			UART0_putstring("Command Unrecognized. Please Try again.");
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			UART0_putchar('>');
			//UART0_putstring(strcat("Error! The following command isn't recognized: ", obc_command));
		} //end else
	} //end while
} //end commandDecode

/*========================================================================================*/
// Function: Update_TELEM
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates telemetry information for OBC. Voltage, current and temperature.
/*========================================================================================*/

void Update_TELEM(double* telem, uint8_t power_state){
	
	// Set 5V rail for readings (needed for current sensors)
	
	// RE-ENABLE FOR DEMOOOOOOOOOOO//////
	Enable_5V();
	
	//Enable_5V();
	// ^^^^^^^^^^^
	
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
	
	// Disable 5V rail
	Disable_5V(power_state);	
}

/*========================================================================================*/
// Function: Update_OBC
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Updates the telemetry information to the OBC.
/*========================================================================================*/

void Update_OBC(double* telem, char* power, uint8_t state_num, uint8_t mode, int soc){
	// Prints a border to the Putty
	char stars[100] = "/********************************************************************CubeSat EPS"; // left side of border
	char stars2[100] = "********************************************************************/"; // right side of border
	char* border; // complete border top and bottom
	border = strcat(stars,stars2);
	
	// Print border
	UART0_putstring(border);
	
	// Spacing
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	
	// String Initializations
	char loads[8][20] = {"Heater 1 (VBatt)", "Heater 2 (5V)", "Heater 3 (8V)", 
						 "OBC", "Imager", "GRIPS", "S-Band", "UHF"};
						 
	char sensors[12][20] = {"3V3-1 Rail [A]", "3V3-2 Rail [A]", "5V Rail [A]", 
							"8V Rail [A]", "Battery Rail [A]", "Solar Input [A]",
							"3V3-1 Rail [V]", "3V3-2 Rail [V]", "5V Rail [V]", "8V Rail [V]",
							"Battery Rail [V]", "Temperature [C]"};
							
	char sections[3][22] = {"CURRENT", "VOLTAGE", "LOADS"};
							
	// Doing to the spacing correctly						
	int spacing = 20;
	int spaces = 0;
	int spaces2 = 0; // for inner loops
	int l_incr = 0; // load increment (8)
	int s_incr = 0; // sensor increment (12)
	int i = 0; // increment for telemetry (0 to 6, then 6 to 12)
	char measure[10]; // used for sprintf
	int section_incr = 0; // used for the section titles
	
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
				sprintf(measure,"%.3f",telem[i]);
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
		if(s_incr == 0){
			UART0_putstring(sections[section_incr]);
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			section_incr++;
		}
		else if(s_incr == 6){
			UART0_putstring(sections[section_incr]);
			UART0_putchar('\n');
			UART0_putchar('\n');
			UART0_putchar('\r');
			section_incr++;
		}
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
				sprintf(measure,"%.2f",telem[i]);
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
	UART0_putstring(sections[section_incr]);
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
		if(power[l_incr] == 'y'){
			UART0_putstring("ON");
			spaces = spacing - 2;			
		}
		else{
			UART0_putstring("OFF");
			spaces = spacing - 3;
		}
		l_incr++;
	}
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	
	// Print the State Number
	char state[5]; // character array for sprintf()
	UART0_putstring("STATE #");
	UART0_putchar('\n');
	UART0_putchar('\r');
	if(state_num == 'M'){
		UART0_putchar('M');
	}
	else{
		sprintf(state,"%d",state_num);
		UART0_putstring(state);
	}
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	
	// Print the State of Charge
	// re-use state from above
	UART0_putstring("STATE OF CHARGE");
	UART0_putchar('\n');
	UART0_putchar('\r');
	sprintf(state,"%d",soc);
	UART0_putstring(state);
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
	
	
	// Print the mode of operation (auto/manual)
	UART0_putstring("MODE");
	UART0_putchar('\n');
	UART0_putchar('\r');
	if(mode){
		UART0_putstring("Autonomous");
	}
	else{
		UART0_putstring("Manual");
	}
	UART0_putchar('\n');
	UART0_putchar('\n');
	UART0_putchar('\r');
}

/*========================================================================================*/
// Function: Manual_Reset
//
// Author: Chris Thomas
// Date: 2019-04-01
// Description: Uses the watchdog timer to reset the system. It is identical launch_reset
//				except the launch bits are not modified.
/*========================================================================================*/

void Manual_RESET(){
	
	// Write to EEPROM
	EEPROM_Write(0xff, 0x0001);
	
	/* Watchdog must be reset to ensure the clock starts at 0.
	/ This is especially important as we are shortening the timeout period
	*/
	
	// Watchdog reset
	wdt_reset();
	
	// Set the watchdog to System Reset Mode
	wdt_enable(SYSTEM_RESET_MODE);
	
	// Change the tiemout period to 1.0s			 
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_2KCLK); // timeout set from 1024k to 128k
	
}
