#ifndef _LED_H
#define _LED_H
  
  #define LED0_ON   GPIOB->ODR &= ~(1<<5);
  #define LED0_OFF  GPIOB->ODR |= (1<<5);
  #define LED1_ON   GPIOE->ODR &= ~(1<<5);
  #define LED1_OFF  GPIOE->ODR |= (1<<5);

  void LED_Init(void);
  void LED_Flash1(void);
  
#endif



