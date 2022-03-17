#include "stm32f4xx.h"   

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/*****************************************************
*函数功能  ：对KEY对应的GPIO口进行初始化设置
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
void KEY_Init(void)
{
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  RCC->AHB1ENR |= (1<<4);
  
  //端口模式寄存器
  GPIOA->MODER &= ~(3<<0);
  GPIOE->MODER &= ~((3<<2*2)|(3<<2*3)|(3<<2*4));
  
  //端口上拉/下拉寄存器
  GPIOA->PUPDR &= ~(3<<0);
  GPIOE->PUPDR &= ~((3<<2*2)|(3<<2*3)|(3<<2*4));
}

/*****************************************************
*函数功能  ：按键扫描函数
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  
  if(((GPIOA->IDR & (1<<0))||!(GPIOE->IDR & (1<<2))||!(GPIOE->IDR & (1<<3))||!(GPIOE->IDR & (1<<4)))&& key_flay)
  {
    delay_ms(10);
    if((GPIOA->IDR & (1<<0))||!(GPIOE->IDR & (1<<2))||!(GPIOE->IDR & (1<<3))||!(GPIOE->IDR & (1<<4)))
    {
      key_flay = 0;
      if(GPIOA->IDR & (1<<0)) temp = 1;
      else if(!(GPIOE->IDR & (1<<2))) temp = 2;
      else if(!(GPIOE->IDR & (1<<3))) temp = 3;
      else if(!(GPIOE->IDR & (1<<4))) temp = 4;
    }
  }
  if(!key_flay && !(GPIOA->IDR & (1<<0))&&(GPIOE->IDR & (1<<2))&&(GPIOE->IDR & (1<<3))&&(GPIOE->IDR & (1<<4)))
  {
    key_flay = 1;
  }
  
  return temp;
}


