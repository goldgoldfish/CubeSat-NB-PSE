/*========================================================================================*/
//
// peripherial_init.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-02-26
// Description: Header file containing all peripheral component initializations for EPS
//				microcontroller.
/*========================================================================================*/

#ifndef PERIPHERAL_INIT_H_
#define PERIPHERAL_INIT_H_

// GPIO Initialization
void GPIO_init();

// ADC Initialization
void ADC_init();

// Clock intialization w/ Interrupt
void CLK_init();

// UART Initialization w/ Interrupt
void UART0_init();

// WatchDog initialization
void WD_init();

#endif