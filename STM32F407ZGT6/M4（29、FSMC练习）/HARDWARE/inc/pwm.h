#ifndef _PWM_H
#define _PWM_H

  #include "stm32f4xx.h" 

  void TIM12_PWM_Init(u16 arr,u16 psc,u16 ccr2);
  void TIM10_PWM_Init(u16 arr,u16 psc,u16 ccr1);

#endif









