/*========================================================================================*/
//
// uart_func.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-06
// Description: Source file containing all functions related to UART serial
//				communication.
/*========================================================================================*/

#include <board.h>
#include <stdlib.h>
#include <stdio.h>

/*========================================================================================*/
// Function: UART0_putchar
//
// Author: Chris Thomas
// Date: 2019-02-15
// Description: Writes a single character to the UART0 data register.
/*========================================================================================*/

void UART0_putchar(char h){
	
	// wait for transmit buffer to be empty
	while(! (UCSR0A & (1 << UDRE0)));
	
	// Put data to be transmitted into buffer
	UDR0 = h; // write to the data register
	
}

/*========================================================================================*/
// Function: UART0_getchar
//
// Author: Chris Thomas
// Date: 2019-02-03
// Description: Reads a single character from the UART0 data register.
/*========================================================================================*/

char UART0_getchar(){
	
	// wait for data to be received
	while (!( UCSR0A & (1 << RXC0)));
	
	// return the received character
	return UDR0;
	
}

/*========================================================================================*/
// Function: UART0_putstring
//
// Author: Chris Thomas
// Date: 2019-03-13
// Description: Writes strings via UART0 using putchar.
/*========================================================================================*/

void UART0_putstring(char *string){
	while(*string){
		UART0_putchar(*string++);
	}
}

/*========================================================================================*/
// Function: UART0_getstring
//
// Author: Chris Thomas
// Date: 2019-03-13
// Description: Reads strings via UART0 using getchar.
/*========================================================================================*/


char* UART0_getstring(){
	char string[50];
	char prev;
	int i = 0;
	
	// Check the first value for the terminator ' ' (SPACE)
	prev = UART0_getchar(); 
	
	// Check previous value for terminator (SPACE)
	while(prev != ' '){
		string[i] = prev; // save the char if it isn't the terminator
		i++;
		prev = UART0_getchar();
	}
	
	return string;
}