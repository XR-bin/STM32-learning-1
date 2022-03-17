#include "sys.h"
#include "stdio.h"
#include "nvic.h"
#include "usart.h"
#include "delay.h"
#include "led.h"

/*****************************************************
*函数功能  ：串口1中断服务函数
*函数名    ：USART1_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART1_IRQHandler(void)
{
  static u8 i = 0;
  //判断中断触发的形式
  if(USART1->SR & (1<<5))
  {
    //中断要执行的任务
    str[i] = USART1->DR;
    i++;
  }
  if(USART1->SR & (1<<4))
  {
    USART1->SR;
    USART1->DR;
    
    str[i] = '\0';
    printf("你发送的数据：%s\r\n",str);
    i = 0;
    cor_flay = 1;
  }
}

/*****************************************************
*函数功能  ：串口2中断服务函数
*函数名    ：USART2_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
            PA2     TX     ---------输出
            PA3     RX     ---------输入
********************************************************/
void USART2_IRQHandler(void)
{
  //判断中断触发的形式
  if(USART2->SR & (1<<5))
  {
    //中断要执行的任务
    data = USART2->DR;
    printf("你收到的指令是：%c\r\n",data);
    flay = 1;
  }
}

/*****************************************************
*函数功能  ：外部中断服务函数
*函数名    ：EXTI4_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
            按键0--------PE4
********************************************************/
void EXTI4_IRQHandler(void)
{
  if(EXTI->PR & (1<<4))
  {
    delay_ms(10);
    if(!(GPIOE->IDR & (1<<4)))
    {
      EXTI->PR |= (1<<4);
      GPIOB->ODR ^= (1<<5);
    }
  }
}

/*****************************************************
*函数功能  ：软件外部中断服务函数
*函数名    ：EXTI9_5_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
            
********************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI->PR & (1<<5))
  {
      EXTI->PR |= (1<<5);
      GPIOB->ODR ^= (1<<5);
  }
}




