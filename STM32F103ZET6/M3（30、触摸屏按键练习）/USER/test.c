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
#include "w25q128.h"
#include "touch.h"
#include "adc.h"
#include "time.h"
#include "pwm.h"
#include "capture.h"
#include "dma.h"
#include "rtc.h"
#include "fsmc.h"
#include "iwdg.h"
#include "can.h"

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
  u8 key;
  
	Stm32_Clock_Init(9);//系统时钟设置
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
	SysTick_Init();	 	//延时初始化
  USART1_Init(115200);
  LED_Init();
  AT24CXX_Init();
  Touch_Init();
  LCD_Init(); 
  AT24CXX_Read_Bytes(0,sizeof(LCD_Touch),(u8 *)&LCD_Touch);
  
  //画矩形
  LCD_Clear_XY(120,120,60,60,RED);
  
  while(1)
  {
    key = LCD_Key_Scan(120,120,60,60);
    if(key == 1) GPIOB->ODR ^= (1<<5);
  }
}




