#include "sys.h"
#include "delay.h"
#include "led.h"

/************************************************************
*函数功能  ：初始化LED灯对应的GPIO口
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            KEY0 PE3
            KEY1 PE4
***************************************************************/
void KEY_Init(void)
{
  //端口时钟使能
  RCC->APB2ENR |= (1<<6);
  //端口模式配置(输入)
  //端口上下拉
  GPIOE->CRL &= ~((0xf<<4*3)|(0xf<<4*4));
  GPIOE->CRL |= ((0x8<<4*3)|(0x8<<4*4));
  GPIOE->ODR |= (3<<3); 
}

/************************************************************
*函数功能  ：初始化LED灯对应的GPIO口
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            KEY0 PE3
            KEY1 PE4
***************************************************************/
u8 KEY_Scan(void)
{
  static u8 temp=0;
  static u8 key_flay = 1;
  if((key_flay)||(!(GPIOE->IDR&(1<<3)))||(!(GPIOE->IDR&(1<<4))))
  {
    delay_ms(10);
    if(!(GPIOE->IDR&(1<<3))||!(GPIOE->IDR&(1<<4)))
    {
      if(!(GPIOE->IDR&(1<<3)))
      {
       // LED1_ON;
        LED0=1;
        temp=1;
      }
      else if(!(GPIOE->IDR&(1<<4)))
      {
        temp = 2;
//        LED1_OFF;
        LED0=0;
      }
      key_flay = 0;
    }
  }
  if(GPIOE->IDR&(1<<3)&&(GPIOE->IDR&(1<<4)))
  {
    key_flay = 1;
  }
  return temp;
}






