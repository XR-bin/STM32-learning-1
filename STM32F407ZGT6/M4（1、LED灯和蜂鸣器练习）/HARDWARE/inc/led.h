#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"
   
   //开灯宏定义
   #define LED0_ON GPIOF->ODR &= ~(1<<6)
   #define LED1_ON GPIOF->ODR &= ~(1<<9)
   #define LED2_ON GPIOF->ODR &= ~(1<<10)
   #define LED3_ON GPIOC->ODR &= ~(1<<0)
   //关灯宏定义
   #define LED0_OFF GPIOF->ODR |= (1<<6)
   #define LED1_OFF GPIOF->ODR |= (1<<9)
   #define LED2_OFF GPIOF->ODR |= (1<<10)
   #define LED3_OFF GPIOC->ODR |= (1<<0)
   
   void LED_Init(void);
   
   void LED_All_ON(void);
   void LED_All_OFF(void);
   
   void LED_X_ON(u8 n);
   void LED_X_OFF(u8 n);
   
   void LED_Flash1(void);
   void LED_Flash2(int n);

#endif








