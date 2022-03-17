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
#include "w25q64.h"
#include "touch.h"

u8 data = 0;
u8 flay = 0;

int main(void)
{   
  u16 x;
  u16 y;
  SysTick_Init();
  USART1_Init(115200);
  Touch_Init();
  
  while(1)
  {
    if(!T_PEN)
    {
      x = Touch_Data(TOUCH_X);
      y = Touch_Data(TOUCH_Y);
      printf("x:%4d    y:%4d\r\n",x,y);
    }
  }
}



















