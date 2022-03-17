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
  u8 key_flay;
  NVIC_SetPriorityGrouping(5);       //优先级分组   
  EXTI5_Swier_Init();
  KEY_Init();
  LED_Init();
  
  while(1)
  {
    key_flay = KEY_Scan();
    if(key_flay == 4)
    {
      EXTI->SWIER |= (1<<5);
    }
  }
  
}











