#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "nvic.h"
#include "exit.h"
#include "delay.h"

u8 data = 0;
u8 flay = 0;

int main(void)
{  
  NVIC_SetPriorityGrouping(5);       //优先级分组  
  SysTick1_Init(500);  
  LED_Init();
  
  while(1)
  {
//    LED_All_ON();
//    delay_ms(100);
//    LED_All_OFF();
//    delay_ms(100);
  }
  
}











