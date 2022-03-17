#include "stm32f4xx.h"  
#include "stdio.h"
#include "capture.h"

/*****************************************************
*函数功能  ：对通用定时器5进行初始化设置并捕获脉冲信号
*函数名    ：TIM5_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc
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

/*************************************************************************
*函数功能  ：对通用定时器9进行初始化设置并捕获脉冲信号
*函数名    ：TIM9_Capture_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc
*描述      ：通过测量脉冲宽度来计算按下时间
*            脉冲输入口 ： PA2   TIM9_CH1  输入通道1  映射通道IC1、IC2
*****************************************************************************/
void TIM9_Capture_Init(u16 arr,u16 psc)
{
  u32 pri;
  
  /*GPIOx端口设置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= 1<<0;
  //端口模式寄存器
  GPIOA->MODER &= ~(3<<(2*2));
	GPIOA->MODER |= (2<<(2*2));
  //上下拉寄存器
  GPIOA->PUPDR &= ~(3<<(2*2));
  //端口复用寄存器
  GPIOA->AFR[0] &= ~(0xf<<(4*2));
	GPIOA->AFR[0] |= (3<<(4*2));
  
  /*TIMx配置*/
  //TIMx时钟使能
  RCC->APB2ENR |= (1<<16);
  //控制寄存器1  CR1
  TIM9->CR1 |= 1 << 7;   //使用重装载影子寄存器
	TIM9->CR1 &= ~(1<<3);  //计数器在发生下一更新事件时停止计数
  TIM9->CR1 &= ~(1<<2);
  TIM9->CR1 &= ~(1<<1);  //允许产生更新，UG位可起作用
  //从模式控制寄存器
  TIM9->SMCR &= ~(7<<4);  
	TIM9->SMCR |=  (5<<4);  //滤波后的定时器输入 1 (TI1FP1)
  TIM9->SMCR &= ~(7<<0);          
	TIM9->SMCR |=  (4<<0);  //复位模式，触发上升沿就清空计数器
  
  /*********ICC1*********/
  //捕获比较模式寄存器1
  TIM9->CCMR1 &= ~(0xf<<4);  
  TIM9->CCMR1 |= (0x3<<4);  //频率最小，次数8次
	TIM9->CCMR1 &= ~(0x3<<0);
	TIM9->CCMR1 |=  (0x1<<0);  //通道CH1进来的信号用通道IC1去捕获
  //捕获比较使能寄存器
  TIM9->CCER &= ~(0x7<<1);   //1位和3位配合配置为上升沿捕获   2位保持复位
  TIM9->CCER |= 1<<0;        //使能捕获
  
  /*********ICC2**********/
  //捕获比较模式寄存器1
  TIM9->CCMR1 &= ~(0xf<<12);  
  TIM9->CCMR1 |= (0x3<<12);   //频率最小，次数8次
	TIM9->CCMR1 &= ~(0x3<<8);
	TIM9->CCMR1 |=  (0x2<<8);   //通道CH1进来的信号用通道IC1去捕获
  //捕获比较使能寄存器
  TIM9->CCER &= ~(0x7<<5);        
	TIM9->CCER |= (0x1<<5);  //5位和7位配合配置为下降沿
  TIM9->CCER |= 1<<4;      //使能捕获
  
  //中断还能寄存器
  TIM9->DIER |= (1<<1); //使能 CC1 中断
	TIM9->DIER |= (1<<2); //使能 CC2 中断
  
  //预分频器
  TIM9->PSC = psc;
  //自动重装载寄存器
  TIM9->ARR = arr;
  //事件生成寄存器
  TIM9->EGR |= 1<<0;
  //清除事件生成标志
  TIM9->SR &= ~(1<0);
  
  /*NVIC配置*/
  //中断分组（在主函数）
  //计算中断优先级编码中断
  pri = NVIC_EncodePriority (5, 1, 2);  //5号分组形式，抢占1级，响应2级
  //将编码写入中断源
  NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,pri);
  //使能NVIC通道
  NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  
  //TIMx使能
  TIM9->CR1 |= (1<<0);
}

/*********************************************
*函数功能  ：定时器9中断函数
*函数名    ：TIM1_BRK_TIM9_IRQHandler ：
*函数参数  ：void     
*函数返回值：void
*函数描述  ：
* 					 判断是定时9的通道1的重复捕获标志
* 							清标志位
* 							读CCR1
*
* 					 判断是定时9的通道2的捕获标志
* 							清标志位
*               读CCR2
*********************************************/
void TIM1_BRK_TIM9_IRQHandler(void)
{
	static u16 data_2;
	u16 data_1;
	u16 p_val;
	
	if(TIM9->SR & (1<<2))
	{
		//清除捕通道IC2的获标志位
		TIM9->SR &= ~(1 << 2);
		//紧急事件
		//CCR2
		data_2 = TIM9->CCR2;	
	}
	
	if(TIM9->SR & (1<<9))
	{
		//清除通道1的捕获标志位
		TIM9->SR &= ~(1 << 1);
		//清除通道1重复捕获标志位
		TIM9->SR &= ~(1 << 9);
		//紧急事件
		//记录值CCR1
		data_1 = TIM9->CCR1;
		//计算周期和占空比
		
		p_val = (float)data_2/data_1*100;
		
		printf("周期：%d   高电平：%d   占空比：%d%%\r\n",data_1,data_2,p_val);	
	}

}

