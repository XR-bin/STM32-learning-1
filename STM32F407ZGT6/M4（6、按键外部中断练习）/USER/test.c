#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "nvic.h"
#include "exit.h"

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

u8 data = 0;
u8 flay = 0;

int main(void)
{  
  NVIC_SetPriorityGrouping(5);       //优先级分组   
  LED_Init();
  EXTI0_Init();
  
  while(1)
  {
    
  }
  
}











