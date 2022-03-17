#include "stm32f4xx.h"   

/*****************************************************
*函数功能  ：对通用定时器12进行初始化设置并输出PWM
*函数名    ：TIM12_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LCD屏亮度
*            PWM输出口 ： PB15   TIM12_CH2  输出通道2
********************************************************/
void TIM12_PWM_Init(u16 arr,u16 psc,u16 ccr2)
{
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= 1<<1;
  //端口模式寄存器
  GPIOB->MODER &= ~(0xc0000000);
  GPIOB->MODER |= (0x80000000);
  //端口输出类型寄存器
  GPIOB->OTYPER &= ~(0x8000);
  //端口输出速度寄存器
  GPIOB->OSPEEDR &= ~(0xc0000000);
  GPIOB->OSPEEDR |= (0x80000000);
  //复用功能寄存器
  GPIOB->AFR[1] &= ~(0xf0000000);
	GPIOB->AFR[1] |= (0x90000000);
  
  /*定时器设置部分*/
  //开启TIMx时钟
  RCC->APB1ENR |= 1<<6;
  //控制寄存器1  CR1
  TIM12->CR1 &= ~(0xffff);
  TIM12->CR1 |= 1<<7;
  //从模式控制寄存器(选择内部时钟源)
  TIM12->SMCR &= ~(7<<0);
  //预分频器
  TIM12->PSC = psc;
  //自动重载寄存器
  TIM12->ARR = arr;
  //捕获/比较模式寄存器1
  TIM12->CCMR1 &= ~(0xff00);
  TIM12->CCMR1 |= 0x6800;
  //捕获/比较使能寄存器
  TIM12->CCER &= ~(0xf0);
  TIM12->CCER |= 1<<5;     //低电平有效
  TIM12->CCER |= 1<<4;
  //捕获/比较寄存器2
  TIM12->CCR2 = ccr2;
  //事件生成寄存器
  TIM12->EGR |= 1<<0;
  //清除事件生成标志
  TIM12->SR &= ~(1<0);
  
  //计算器使能
  TIM12->CR1 |= 1<<0;
}

/*****************************************************
*函数功能  ：对通用定时器10进行初始化设置并输出PWM
*函数名    ：TIM10_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LED亮度
*            PWM输出口 ： PF6   TIM10_CH1  输出通道1
********************************************************/
void TIM10_PWM_Init(u16 arr,u16 psc,u16 ccr1)
{
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= 1<<5;
  //端口模式寄存器
  GPIOF->MODER &= ~(3<<(2*6));
  GPIOF->MODER |= (2<<(2*6));
  //端口输出类型寄存器
  GPIOF->OTYPER &= ~(1<<6);
  //端口输出速度寄存器
  GPIOF->OSPEEDR &= ~(3<<(2*6));
  GPIOF->OSPEEDR |= (2<<(2*6));
  //复用功能寄存器
  GPIOF->AFR[0] &= ~(0xf<<(4*6));
	GPIOF->AFR[0] |= (3<<(4*6));
  
  /*定时器设置部分*/
  //开启TIMx时钟
  RCC->APB2ENR |= 1<<17;
  //控制寄存器1  CR1
  TIM10->CR1 &= ~(0xffff);
  TIM10->CR1 |= 1<<7;
  //预分频器
  TIM10->PSC = psc;
  //自动重载寄存器
  TIM10->ARR = arr;
  //捕获/比较模式寄存器1
  TIM10->CCMR1 &= ~(0xff);
  TIM10->CCMR1 |= 0x68;
  //捕获/比较使能寄存器
  TIM10->CCER &= ~(0xf);
  TIM10->CCER |= (1<<0);     //使能通道1
	TIM10->CCER |= (1<<1);     //低电平有效
  //捕获/比较寄存器1
  TIM10->CCR1 = ccr1;
  //事件生成寄存器
  TIM10->EGR |= 1<<0;
  //清除事件生成标志
  TIM10->SR &= ~(1<0);
  
  //计算器使能
  TIM10->CR1 |= 1<<0;
}


       











