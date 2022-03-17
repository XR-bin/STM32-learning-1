#ifndef _CAN_H
#define _CAN_H

  #include "stm32f4xx.h" 

  void CAN1_Init(void);
  void CAN1_Send_Byte(u8 data);
  void CAN1_Receive_Byte(void);

#endif









