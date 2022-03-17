#ifndef _LED_H
#define _LED_H
  
  #define LED1_ON   GPIOF->ODR &= ~(1<<6)
  #define LED2_ON   GPIOF->ODR &= ~(1<<9)
  #define LED3_ON   GPIOF->ODR &= ~(1<<10)
  #define LED4_ON   GPIOC->ODR &= ~(1<<0)
  
  #define LED1_OFF  GPIOF->ODR |= (1<<6)
  #define LED2_OFF  GPIOF->ODR |= (1<<9)
  #define LED3_OFF  GPIOF->ODR |= (1<<10)
  #define LED4_OFF  GPIOC->ODR |= (1<<0)
  
  void LED_Init(void);

#endif








