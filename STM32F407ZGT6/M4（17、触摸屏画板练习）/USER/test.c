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

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
*         2、AT24C02的0~16用于存储LCD与触摸屏之间的校准系数
***************************************************************/

extern ADJUST LCD_Touch;  

int main(void)
{ 
  TOUCH_XY lcd;
  
  SysTick_Init();
  USART1_Init(115200);
  AT24CXX_Init();
  Touch_Init();
  LCD_Init(); 
  AT24CXX_Read_Bytes(0,sizeof(LCD_Touch),(u8 *)&LCD_Touch);
  while(1)
  {
    if(!T_PEN)
    {
      lcd = LCD_Touch_Transition();
      LCD_Circle2(lcd.x,lcd.y,3,RED);
    }
  }
}



















