#ifndef _IIC_H
#define _IIC_H

  #include "stm32f4xx.h"
  
  //SCL:
  #define SCL_H      (GPIOB->ODR |= (1<<8))    //时钟线拉高
  #define SCL_L      (GPIOB->ODR &= ~(1<<8))   //时钟线拉低
  //SDA
  //输出：
  #define SDA_OUT_H  (GPIOB->ODR |= (1<<9))    //输出模式下数据线拉高
  #define SDA_OUT_L  (GPIOB->ODR &= ~(1<<9))   //输出模式下数据线拉低
  //输入：
  #define SDA_IN     (GPIOB->ODR |= (1<<9))    //输入模式
  #define SDA_INT    (GPIOB->IDR & (1<<9))     //接收到的值

  void IIC_Init(void);
  void IIC_Start(void);
  void IIC_Stop(void);
  void IIC_Send_Ack(u8 ack);
  u8 IIC_Receive_Ack(void);
  void IIC_Send_Byte(u8 data);
  u8 IIC_Receive_Byte(void);
  
#endif





