#include "stm32f4xx.h"  

/*****************************************************
*函数功能  ：对定时器6进行初始化设置
*函数名    ：TIM6_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM6_Init(u16 arr,u16 psc)
{
  u32 pri;
  //TIMx时钟使能
  RCC->APB1ENR |= 1<<4;
  //控制寄存器CR1
  TIM6->CR1 &= ~(0xffff);
  TIM6->CR1 |= 1<<7;
  //中断寄存器
  TIM6->DIER &= ~(0xffff);
  TIM6->DIER |= 1<<0;
  //预分频器
  TIM6->PSC = psc;
  //自动重装载寄存器
  TIM6->ARR = arr * 10;
  //更新事件生成寄存器(人为产生一个更新)
  TIM6->EGR |= 1<<0;
  //清除更新事件标志
  TIM6->SR &= ~(1<0);
  
  /*NVIC配置*/
  //中断分组（在主函数）
  //计算中断优先级编码中断
  pri = NVIC_EncodePriority (5, 1, 2);  //5号分组形式，抢占1级，响应2级
  //将编码写入中断源
  NVIC_SetPriority(TIM6_DAC_IRQn,pri);
  //使能NVIC通道
  NVIC_EnableIRQ(TIM6_DAC_IRQn); 
  
  //开计数器
	TIM6->CR1 |= (1<<0);
}













