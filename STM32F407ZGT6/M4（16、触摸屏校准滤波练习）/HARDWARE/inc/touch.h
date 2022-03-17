#ifndef _TOUCH_H
#define _TOUCH_H

  #include "stm32f4xx.h"  
  
  //用于普通的触摸屏x和y坐标读取
  typedef struct touch
  {
    u16 x;
    u16 y;
  }TOUCH_XY;
 //用于接收LCD和触摸屏的关系量
 typedef struct adjust
 {
   float a_x;  //lcd 和 触摸屏 x坐标的比例系数
   signed short b_x;    //lcd 和 触摸屏 x坐标的偏移量
   float c_y;  //lcd 和 触摸屏 y坐标的比例系数
   signed short d_y;    //lcd 和 触摸屏 y坐标的偏移量
 }ADJUST;
 
  //片选线
  #define T_CS_H    GPIOC->ODR |= (1<<13)
  #define T_CS_L    GPIOC->ODR &= ~(1<<13)
  //时钟线
  #define T_SCK_H   GPIOB->ODR |= (1<<0)
  #define T_SCK_L   GPIOB->ODR &= ~(1<<0)
  //输出线
  #define T_MOSI_H  GPIOF->ODR |= (1<<11)
  #define T_MOSI_L  GPIOF->ODR &= ~(1<<11)
  //输入线
  #define T_MISO    (GPIOB->IDR & (1<<2))
  //笔接触线
  #define T_PEN     (GPIOB->IDR & (1<<1))
  
  //指令
  #define TOUCH_X   0xD0    //获取x坐标数据
  #define TOUCH_Y   0x90    //获取y坐标数据
  
  void Touch_Init(void);
  u16 Touch_Data(u8 command);
  TOUCH_XY Touch_Smoothing(void);
  void LCD_Calibration_Cross(u16 x,u16 y,u16 colour);
  void LCD_Touch_Calibration(void);
  TOUCH_XY LCD_Touch_Transition(void);
	
#endif




