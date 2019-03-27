/*========================================================================================*/
//
// gpio_func.c
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-15
// Description: Source file containing all functions related to GPIO controlled
//				subsystems.
/*========================================================================================*/

#include "gpio_func.h"
#include <board.h>
#include <stdlib.h>
#include <stdio.h>

/*========================================================================================*/
// Function: Set_GPIO
//
// Author: Chris Thomas
// Date: 2019-03-15
// Description: Writes a 1 (HIGH) to the corresponding GPIO pin.
/*========================================================================================*/

void Set_GPIO(int gpio_num){
	
	int gpio = 0x01; // initialize gpio selection to bit 0
	
	// PORT H
	if (gpio_num < 7){
		gpio = gpio << gpio_num; // shift to correct bit
		PORTH |= gpio; // set selected pin high
	}
	// PORT L
	else if (gpio_num >= 7 && gpio_num < 10){
		gpio = gpio << (gpio_num - 6); // shift to correct bit
		PORTL |= gpio; // set selected pin high
	}
	// PORT A
	else if(gpio_num >= 10 && gpio_num < 14){
		gpio = gpio << (gpio_num - 6); // shift to correct bit
		PORTA |= gpio; // set selected pin high
	}
	// PORT J
	else if(gpio_num >= 14 && gpio_num < 21){
		gpio = gpio << (gpio_num - 14); // shift to correct bit
		PORTJ |= gpio; // set selected pin high
	}
	// PORT C
	else if (gpio_num >= 21 && gpio_num < 29){
		gpio = gpio << (gpio_num - 21); // shift to correct bit
		PORTC |= gpio; // set selected pin high
	}
	// Otherwise do nothing
	else{
		// do nothing
	}
	
}

/*========================================================================================*/
// Function: Clear_GPIO
//
// Author: Chris Thomas
// Date: 2019-03-15
// Description: Writes a 0 (LOW) to the corresponding GPIO pin.
/*========================================================================================*/


void Clear_GPIO(int gpio_num){
	
	int gpio = 0x01; // initialize gpio selection to bit 0
	int gpio_mask = 0xFE; //gpio mask
	
	// PORT H
	if (gpio_num < 7){
		gpio = gpio << gpio_num; // shift to correct bit
		PORTH &= (gpio^gpio_mask); // use mask to clear bit
	}
	// PORT L
	else if (gpio_num >= 7 && gpio_num < 10){
		gpio = gpio << (gpio_num - 6); // shift to correct bit
		PORTL &= (gpio^gpio_mask); // use mask to clear bit
	}
	// PORT A
	else if(gpio_num >= 10 && gpio_num < 14){
		gpio = gpio << (gpio_num - 6); // shift to correct bit
		PORTA &= (gpio^gpio_mask); // use mask to clear bit
	}
	// PORT J
	else if(gpio_num >= 14 && gpio_num < 21){
		gpio = gpio << (gpio_num - 14); // shift to correct bit
		PORTJ &= (gpio^gpio_mask); // use mask to clear bit
	}
	// PORT C
	else if (gpio_num >= 21 && gpio_num < 29){
		gpio = gpio << (gpio_num - 21); // shift to correct bit
		PORTC &= (gpio^gpio_mask); // use mask to clear bit
	}
	// Otherwise do nothing
	else{
		// do nothing
	}
	
}

