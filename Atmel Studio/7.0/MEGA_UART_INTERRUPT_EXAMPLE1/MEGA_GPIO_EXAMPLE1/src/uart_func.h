/*========================================================================================*/
//
// uart_func.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Header file containing all functions related to UART serial
//				communication.
/*========================================================================================*/


#ifndef UART_FUNC_H_
#define UART_FUNC_H_

// Writes a single character to the UART0 data register
void UART0_putchar(char h);

// Reads a single character from the UART0 data register
char UART0_getchar();

// Writes strings via UART0 using putchar
void UART0_putstring(char *string);

// Reads strings via UART0 using getchar
void UART0_getstring(char* message);


#endif /* UART_FUNC_H_ */