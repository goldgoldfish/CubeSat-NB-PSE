/*========================================================================================*/
//
// gpio_func.h
//
// ENGG4000
// Team 02 - CubeSatNB Electrical Power Supply
//
// Author: Chris Thomas
// Date: 2019-03-15
// Description: Header file containing all functions related to GPIO controlled
//				subsystems.
/*========================================================================================*/


#ifndef GPIO_FUNC_H_
#define GPIO_FUNC_H_

// Set the specified GPIO pin to 1 (HIGH)
void Set_GPIO(int gpio_num);

// Clear the specified GPIO to 0 (LOW)
void Clear_GPIO(int gpio_num);

#endif /* GPIO_FUNC_H_ */