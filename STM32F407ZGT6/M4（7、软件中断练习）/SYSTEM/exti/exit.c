#include "stm32f4xx.h" 

/******************************************************************
*函数功能  ：外部中断EXIT0触发初始化
*函数名    ：EXTI0_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             PA0按键       上升沿触发
*******************************************************************/
void EXTI0_Init(void)
{
  u8 pri;
  /*GPIOx配置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  //端口模式寄存器
  GPIOA->MODER &= ~(3<<0);
  //端口上下拉
  GPIOA->PUPDR &= ~(3<<0);
  //端口输出数据寄存器
  GPIOA->ODR &= ~(1<<0);
  
  /*EXTI控制器配置*/
  //系统配置控制器时钟使能
  RCC->APB2ENR |= (1<<14);
  //映射管脚
  SYSCFG->EXTICR[0] &= ~(0xf<<0);
  //上升沿/下降沿寄存器配置
  EXTI->RTSR &= ~(1<<0);
  EXTI->RTSR |= (1<<0);
  
  /*NVIC控制器配置*/
  //优先级分组（写在主函数）
  //计算优先级编码值
  pri = NVIC_EncodePriority (5, 1, 1);
  //将编码值写入具体中断源
  NVIC_SetPriority(EXTI0_IRQn,pri);
  //使能NVIC响应通道
  NVIC_EnableIRQ(EXTI0_IRQn);
  
  //中断屏蔽器
  EXTI->IMR |= (1<<0);
}

/******************************************************************
*函数功能  ：软件外部中断发初始化
*函数名    ：EXTI5_Swier_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             无需端口
*******************************************************************/
void EXTI5_Swier_Init(void)
{
  u8 pri;
  //软件中断寄存器
  EXTI->SWIER |= (1<<5);
  
  /*NVIC控制器配置*/
  //优先级分组（写在主函数）
  //计算优先级编码值
  pri = NVIC_EncodePriority (5, 1, 1);
  //将编码值写入具体中断源
  NVIC_SetPriority(EXTI9_5_IRQn,pri);
  //使能NVIC响应通道
  NVIC_EnableIRQ(EXTI9_5_IRQn);
  
  //中断屏蔽器
  EXTI->IMR |= (1<<5);
}








