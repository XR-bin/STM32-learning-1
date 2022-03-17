#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"
#include "string.h"
#include "led.h"
#include "w25q64.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

u32 temp = 1;

/******************************************************************
*函数功能  ：串口1的中断服务函数
*函数名    ：USART1_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART1_IRQHandler(void)
{
  u8 data;
  static u32 addr = 0x0b0000;
  
  //判断是由什么形式触发中断
  if(USART1->SR & (1<<5))
  {
    //执行中断任务
    data = USART1->DR;
    W25Q64_Page_Write(addr,1,&data);
    addr++;
    LED1_ON;
  }
  if(USART1->SR & (1<<4))
  {
    USART1->SR;
    USART1->DR;
    LED1_OFF;
  }
}

/******************************************************************
*函数功能  ：串口2的中断服务函数
*函数名    ：USART2_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART2_IRQHandler(void)
{
  
  //判断是由什么形式触发中断
  if(USART2->SR & (1<<5))
  {
    //执行中断任务
    data = USART2->DR;
    flay = 1;
  }
}

/******************************************************************
*函数功能  ：外部中断1服务函数
*函数名    ：EXTI0_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void EXTI0_IRQHandler(void)
{
  if(EXTI->PR & (1<<0))
  {
    delay_ms(10);
    if((GPIOA->IDR & (1<<0)))
    {
      EXTI->PR |= (1<<0);
      GPIOF->ODR ^= (1<<6);
      GPIOF->ODR ^= (1<<9);
      GPIOF->ODR ^= (1<<10);
      GPIOC->ODR ^= (1<<0);
    }
  }
}

void SysTick_Handler(void)
{
  if(SysTick->CTRL & (1<<16))
  {
    //将当前值寄存器清零   //随便写一个数据进入
    SysTick->VAL = 0xff;
    GPIOF->ODR ^= (1<<6);
  }
}
