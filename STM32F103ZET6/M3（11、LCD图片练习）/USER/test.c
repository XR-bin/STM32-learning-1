#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h" 
#include "nvic.h"
#include "string.h"
#include "exti.h"
#include "at24cxx.h"
#include "stdio.h"
#include "lcd.h"
#include "tu.h"
#include "tu2.h"

u8 cor_flay = 0;
u8 str[200];
u8 data = 0;
u8 flay = 0;

int main(void)
{	
  u8 key;
  
	Stm32_Clock_Init(9);//系统时钟设置
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
	SysTick_Init();	 	//延时初始化
  USART1_Init(9600);
  KEY_Init();
  LCD_Init();
  LCD_Picture(0,0,(u8 *)gImage_tu);
  while(1)
  {
    key = KEY_Scan();
    if(key == 3)
    {
      LCD_Picture(0,0,(u8 *)gImage_tu);
    }
    else if(key == 2)
    {
      LCD_Picture2(0,0,(u8 *)gImage_tu2);
    }
  }
}




