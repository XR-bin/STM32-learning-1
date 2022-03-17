#ifndef _BEEP_H
#define _BEEP_H

  #define BEEP_ON  GPIOB->ODR |= (1<<8);
  #define BEEP_OFF GPIOB->ODR &= ~(1<<8); 
  
  void BEEP_Init(void);

#endif










