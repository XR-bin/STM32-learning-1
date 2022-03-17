#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "nvic.h"
#include "exit.h"
#include "delay.h"
#include "at24cxx.h"
#include "lcd.h"

u8 data = 0;
u8 flay = 0;

int main(void)
{  
  SysTick_Init();
  NVIC_SetPriorityGrouping(5);       //优先级分组 
  USART1_Init(115200);
  LCD_Init();
  LCD_Character16(84,300,'A',GREEN);
  LCD_Character24(100,300,'A',BLUE);
  LCD_Character32(124,300,'A',RED);
  LCD_Characters_String32(200,200,"方思虹",RED);
  while(1)
  {
  }
}











