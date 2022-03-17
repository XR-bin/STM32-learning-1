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

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
*         2、AT24C02的0~16用于存储LCD与触摸屏之间的校准系数
***************************************************************/

u8 cor_flay = 0;
u8 str[200];
u8 data = 0;
u8 flay = 0;

int main(void)
{		 
  u8 str[] = "momo没哈哈";
  u8 str2[30]; 
	Stm32_Clock_Init(9);//系统时钟设置
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
	SysTick_Init();	 	//延时初始化
  USART1_Init(9600);
  //LCD_Init();
  W25Q128_Init();
  W25Q128_Sector_Erase(0x5);
  W25Q128_Page_Write(0x1,sizeof(str),str);
  W25Q128_Read_Bytes(0x1,sizeof(str),str2);
	while(1)
	{
    printf("str2 : %s\r\n",str2);
    delay1_ms(500);
  }
}




