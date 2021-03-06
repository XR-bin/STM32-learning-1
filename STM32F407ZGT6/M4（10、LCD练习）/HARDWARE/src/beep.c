#include "stm32f4xx.h"     

/*****************************************************
*函数功能  ：对BEEP对应的GPIO口进行初始化设置
*函数名    ：BEEP_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             BEEP  PF8
             高电响，低电不响-----输出模式
********************************************************/
void BEEP_Init(void)
{
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<5);
  
  //端口模式寄存器
  GPIOF->MODER &= ~(3<<2*8);
  GPIOF->MODER |= (1<<2*8);
  
  //端口输出类型寄存器
  GPIOF->OTYPER &= ~(1<<8);
  
  //端口输出速率寄存器
  GPIOF->OSPEEDR &= ~(3<<2*8);;
  GPIOF->OSPEEDR |= (2<<2*8);
  
  //端口输出数据寄存器
  GPIOF->ODR &= ~(1<<8);
}













