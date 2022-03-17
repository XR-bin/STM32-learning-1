#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "nvic.h"

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

int main(void)
{  
  
  NVIC_SetPriorityGrouping(5);       //优先级分组 
  USART1_Init(115200);
  LED_Init();
  KEY_Init();   
  
  while(1)
  {
    LED_Flash1();
  }
  
}











