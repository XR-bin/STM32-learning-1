#ifndef _TOUCH_H
#define _TOUCH_H

  #include "sys.h"

  //片选线
  #define T_CS_H    GPIOF->ODR |= (1<<11)
  #define T_CS_L    GPIOF->ODR &= ~(1<<11)
  //时钟线
  #define T_SCK_H   GPIOB->ODR |= (1<<1)
  #define T_SCK_L   GPIOB->ODR &= ~(1<<1)
  //输出线
  #define T_MOSI_H  GPIOF->ODR |= (1<<9)
  #define T_MOSI_L  GPIOF->ODR &= ~(1<<9)
  //输入线
  #define T_MISO    (GPIOB->IDR & (1<<2))
  //笔接触线
  #define T_PEN     (GPIOF->IDR & (1<<10))
  
  //指令
  #define TOUCH_X   0xD0    //获取x坐标数据
  #define TOUCH_Y   0x90    //获取y坐标数据
  
  void Touch_Init(void);
  u16 Touch_Data(u8 command);

#endif







