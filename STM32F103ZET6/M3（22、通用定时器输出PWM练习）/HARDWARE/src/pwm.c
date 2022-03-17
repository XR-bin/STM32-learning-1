#include "sys.h"

/*****************************************************
*函数功能  ：对通用定时器3进行初始化设置并输出PWM
*函数名    ：TIM3_PWM_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过PWM来控制LED0屏亮度
*            PWM输出口 ： PB5   TIM3_CH2  输出通道2
********************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc,u16 ccr2)
{
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<3;
  //端口配置寄存器
  GPIOB->CRL &= ~(0xf<<(4*5));
  GPIOB->CRL |= (0xb<<(4*5));
  //复用重映射寄存器
  RCC->APB2ENR |= 1<<0;
  AFIO->MAPR &= ~(3<<10);
  AFIO->MAPR |= (2<<10);
  
  /*定时器设置部分*/
  //开启TIMx时钟
  RCC->APB1ENR |= 1<<1;
  //控制寄存器1  CR1
  TIM3->CR1 &= ~(0xffff);
  TIM3->CR1 |= 1<<7;
  //从模式控制寄存器(选择内部时钟源)
  TIM3->SMCR &= ~(7<<0);
  //预分频器
  TIM3->PSC = psc;
  //自动重载寄存器
  TIM3->ARR = arr;
  //捕获/比较模式寄存器1
  TIM3->CCMR1 &= ~(0xff00);
  TIM3->CCMR1 |= 0x6800;
  //捕获/比较使能寄存器
  TIM3->CCER &= ~(0xf0);
  //TIM3->CCER |= 1<<5;
  TIM3->CCER |= 1<<4;
  //捕获/比较寄存器2
  TIM3->CCR2 = ccr2;
  //事件生成寄存器
  TIM3->EGR |= 1<<0;
  //清除事件生成标志
  TIM3->SR &= ~(1<0);
  
  //计算器使能
  TIM3->CR1 |= 1<<0;
}










