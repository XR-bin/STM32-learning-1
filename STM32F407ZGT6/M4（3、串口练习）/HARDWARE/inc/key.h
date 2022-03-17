#ifndef _KEY_H
#define _KEY_H

  #include "stm32f4xx.h"  

  #define KEY1 GPIOA->IDR
  #define KEY2 GPIOE->IDR
  
  void KEY_Init(void);
  u8 KEY_Scan(void);
  u8 KEY_Scan2(void);
  
#endif



