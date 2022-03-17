#ifndef _BEEP_H
#define _BEEP_H

  #define BEEP_OFF GPIOF->ODR &= ~(1<<8)
  #define BEEP_ON  GPIOF->ODR |= (1<<8)
  
  void BEEP_Init(void);
  void BEEP_ON_OFF(int i);

#endif













