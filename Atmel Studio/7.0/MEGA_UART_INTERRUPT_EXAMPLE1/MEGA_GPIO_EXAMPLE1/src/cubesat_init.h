/*
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
// 
// Author: Chris Thomas
// Date: 2019-02-26 
// Description: Header file containing all peripheral initialization for
//				EPS microcontroller.
//
*/



/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-15
// Description: General Purpose I/O Port initialization
/*========================================================================================*/

void GPIO_init(){
	
	DDRC |= 0x1f; // make PINC0-PINC4 outputs

	PORTC |= 0x00; // turn off all LEDS
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-01-22
// Description: Analog to Digital Converter initialization
/*========================================================================================*/

void ADC_init(){
	// Enable PortF
	
	// Enable as Input
	DDRF = 0x00;
	
	// Set Prescaller
	
	
	// Enable the 1.1V internal reference
	//ADMUX |= ((REFS1 << 0) | (REFS0 << 0)); // set the first bit to 1
	ADMUX = 0x00;
	
	// Enable the ADC via the status control register & enable interrupt
	//ADCSRA |= (ADEN << 1) | (ADPS2 << 1) | (ADPS0 << 1) | (ADIE << 1);
	ADCSRA = 0x85;
	
	// Enable ADC0
	//ADMUX |= ((0 << MUX0) | (0 << MUX1) | (0 << MUX2) | (0 << MUX3) | (0 << MUX4)); // ADC0
	//ADCSRB |= (0 << MUX5); // ADC0
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-07
// Description: Timer Initialization for Interrupt Routine
/*========================================================================================*/

CLK_init(){
	
	// Enable Timer0
	TCCR1B = 0x05;
	
	// Enable Output Compare Interrupt
	TIMSK1 = 0x02;
	
	// Set Counter to 128 clock cycles
	OCR1A = 0xffff;
	
	// Set clock to 0
	TCNT1 = 0x0000;
	
}

/*========================================================================================*/
// Author: Chris Thomas
// Date: 2019-02-16
// Description: UART Initialization
/*========================================================================================*/

void UART0_init(){
	
	// Using the values from set baud tool
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	// Enable TX & RX and Enable RX complete interrupt
	UCSR0B = 0x98; // enabling bits 4 & 3
	
	
	// Set up the format 8bit no parity
	UCSR0C = 0x06; // enable 8 bit chars
	
}