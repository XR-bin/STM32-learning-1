#ifndef _LED_H
#define _LED_H

 #include "sys.h"

 #define LED0_ON  GPIOB->ODR &= ~(1<<5);
 #define LED1_ON  GPIOE->ODR &= ~(1<<5);
 #define LED0_OFF GPIOB->ODR |= (1<<5);
 #define LED1_OFF GPIOE->ODR |= (1<<5);
 
#define LED0 PBout(5)	// DS0
#define LED1 PEout(5)	// DS1	
 

 
 void LED_Init(void);

#endif









