#include "sys.h"
#include "stdio.h"

void USART1_IRQHandler(void)
{
  u8 data;
  //判断中断触发的形式
  if(USART1->SR & (1<<5))
  {
    //中断要执行的任务
    data = USART1->DR;
    printf("你发送的数据：%c\r\n",data);
  }
}









