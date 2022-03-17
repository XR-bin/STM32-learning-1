#include "sys.h"

/*****************************************************
*函数功能  ：外部中断初始化
*函数名    ：EXTI4_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            按键0--------PE4 低电平按下，高电平抬起
********************************************************/
void EXTI4_Init(void)
{
  u8 pri;
  
  /*GPIOx初始化设置*/
  //GPIOx时钟使能
  RCC->APB2ENR |= (1<<6);
  //端口配置寄存器
  GPIOE->CRL &= ~(0xf<<(4*4));
  GPIOE->CRL |= (8<<(4*4));
  //端口输出寄存器  设置高电平：正式上拉 设置低电平：正式下拉
  GPIOE->ODR |= (1<<4);
  
  /*外部中断配置*/
  //AFIO时钟使能
  RCC->APB2ENR |= (1<<0);
  //外部中断配置寄存器
  AFIO->EXTICR[1] &= ~(0xf<<0);
  AFIO->EXTICR[1] |= (4<<0);
  //上升/下降沿触发选择寄存器
  EXTI->RTSR |= (1<<4);
  
  /*中断分组*/
  //优先级分组（主函数里写）
  //计算优先级编码值
  pri = NVIC_EncodePriority (5, 1, 1);
  //将编码值写入具体中断源
  NVIC_SetPriority(EXTI4_IRQn,pri);
  //使能NVIC响应通道
  NVIC_EnableIRQ(EXTI4_IRQn); 
  
  //中断屏蔽寄存器
  EXTI->IMR |= (1<<4);
}













