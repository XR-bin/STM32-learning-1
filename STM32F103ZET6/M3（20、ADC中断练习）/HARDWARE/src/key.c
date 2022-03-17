#include "sys.h"
#include "delay.h"

/*****************************************************
*函数功能  ：对KEY对应的GPIO口进行初始化设置
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             KEY0     PE4
             KEY1     PE3
             KEY_UP   PA0
             KEY0、KEY1 低电平按下，高电平抬起
             KEY_UP     高电平按下，低电平抬起
                                  -------------输出模式
********************************************************/
void KEY_Init(void)
{
  //GPIOx时钟使能
  RCC->APB2ENR |= ((1<<2)|(1<<6));
  
  //端口配置寄存器
  GPIOA->CRL &= ~(0xf<<4*0);
  GPIOA->CRL |= (8<<4*0);
  
  GPIOE->CRL &= ~((0xf<<4*3)|(0xf<<4*4));
  GPIOE->CRL |= ((8<<4*3)|(8<<4*4));
  
  //端口输出寄存器  设置高电平：正式上拉 设置低电平：正式下拉
  GPIOA->ODR &= ~(1<<0);
  GPIOE->ODR |= ((1<<3)|(1<<4));
}

/*****************************************************
*函数功能  ：按键扫描函数
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
             KEY0     PE4
             KEY1     PE3
             KEY_UP   PA0
             KEY0、KEY1 低电平按下，高电平抬起
             KEY_UP     高电平按下，低电平抬起
                                  -------------输出模式
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay && ((GPIOA->IDR & (1<<0))||!(GPIOE->IDR & (1<<3))||!(GPIOE->IDR & (1<<4))))
  {
    delay_ms(10);
    if((GPIOA->IDR & (1<<0))||!(GPIOE->IDR & (1<<3))||!(GPIOE->IDR & (1<<4)))
    {
      key_flay = 0;
      if(!(GPIOE->IDR & (1<<4)))     temp = 1;
      else if(!(GPIOE->IDR & (1<<3))) temp = 2;
      else if(GPIOA->IDR & (1<<0))    temp = 3;
    }
  }
  if(!(GPIOA->IDR & (1<<0)) && (GPIOE->IDR & (1<<3)) && (GPIOE->IDR & (1<<4)))
  {
    key_flay = 1;
  }
  return temp;
}




