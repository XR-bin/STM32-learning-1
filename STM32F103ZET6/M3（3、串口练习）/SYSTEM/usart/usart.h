#ifndef _USART_H
#define _USART_H

  #include "sys.h"
  
  void USART1_Init(u32 baud);
  void USART1_Send_Byte(u8 data);
  u8 USART1_Receive_Byte(void);

#endif














