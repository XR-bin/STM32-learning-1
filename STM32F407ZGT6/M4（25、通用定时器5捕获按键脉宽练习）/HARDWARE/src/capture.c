#include "stm32f4xx.h"  
#include "stdio.h"

/*****************************************************
*函数功能  ：对通用定时器5进行初始化设置并捕获脉冲信号
*函数名    ：TIM5_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc,u16 ccr2
*描述      ：通过测量脉冲宽度来计算按下时间
*            脉冲输入口 ： PA0   TIM5_CH1  输入通道1
********************************************************/
void TIM5_Capture_Init(u16 arr,u16 psc)
{
  u32 pri;
  
  /*GPIOx寄存器配置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  //端口模式寄存器
  GPIOA->MODER &= ~(3<<0);
  GPIOA->MODER |= (2<<0);
  //上拉下拉寄存器
  GPIOA->PUPDR &= ~(3<<0);
  //复用功能寄存器
  GPIOA->AFR[0] &= ~(0xf<<0);
  GPIOA->AFR[0] |= (2<<0);
  
  /*TIMx寄存器配置*/
  //TIMx时钟使能
  RCC->APB1ENR |= (1<<3);
  //控制寄存器1  CR1
  TIM5->CR1 &= ~(0xffff);
  TIM5->CR1 |= (2<<8);
  TIM5->CR1 |= (1<<7);
  //从模式控制寄存器
  TIM5->SMCR &= ~(0xffff);
  //捕获比较模式寄存器1
  TIM5->CCMR1 &= ~(0xff<<0);
  TIM5->CCMR1 |= (1<<0);   //将通道1设置为捕获
	TIM5->CCMR1 |= (0xf<<4); //8次采样，采样频率最低
  //预分频器
  TIM5->PSC = psc;
  //自动重装载寄存器
  TIM5->ARR = arr;
  //捕获比较使能寄存器
  TIM5->CCER &= ~(0x7<<0);
  TIM5->CCER |= (0x1<<0);
  //中断还能寄存器
  TIM5->DIER |= 1<<1;   //上升沿触发
  TIM5->DIER |= 1<<0;   //更新事件触发
  
  //事件生成寄存器
  TIM5->EGR |= 1<<0;
  //清除事件生成标志
  TIM5->SR &= ~(1<0);
  
  /*NVIC配置*/
  //中断分组（在主函数）
  //计算中断优先级编码中断
  pri = NVIC_EncodePriority (5, 1, 2);  //5号分组形式，抢占1级，响应2级
  //将编码写入中断源
  NVIC_SetPriority(TIM5_IRQn,pri);
  //使能NVIC通道
  NVIC_EnableIRQ(TIM5_IRQn);
  
  //TIMx使能
  TIM5->CR1 |= (1<<0);
}

/******************************************
*函数功能  ：定时器5的中断服务函数
*函数名    ：TIM5_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM5_IRQHandler(void)
{
  static u16 update_num;
  static u8 update_flay = 0;
  static u16 rising_edge; 
  u16 falling_edge;
  u16 cont; 
  u16 t;
	//更新中断
	if(TIM5->SR & (1<<0))
	{
		//清中断标志位
		TIM5->SR &= ~(1<<0);
		//紧急事件
    if(update_flay)
    {
      update_num++;
    }
	}
  //边沿触发中断
  if(TIM5->SR & (1<<1))
  {
    //判断是不是上升沿触发
    if(!(TIM5->CCER & (1<<1)))
    {
      //开启溢出计数标志位
      update_flay = 1;
      //获取上升沿计数
      rising_edge = TIM5->CCR1;
      //把边沿触发形式改为下降沿
      TIM5->CCER |= (1<<1);	
    }
    else if(TIM5->CCER & (1<<1))
    {
      //获取下升沿计数
      falling_edge = TIM5->CCR1;
      //计算脉冲宽度
			cont = 65535 * update_num - rising_edge + falling_edge;
      t = cont / 10;
      
      printf("脉冲宽度：%d\r\n",t);
      
      update_num = 0;
      update_flay = 0;
      
      //切换成上升沿
			TIM5->CCER &= ~(1<<1);
    }
  }
}









