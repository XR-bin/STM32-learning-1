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
#include "adc.h"

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
  ADC2_DATA val;
  SysTick_Init();
  USART1_Init(115200);
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
  ADC2_Init_In4In5();

  while(1)
  {
    val = ADC2_In4In5_Data();
    printf("滑动电阻：%d  光敏电阻：%d\r\n",val.adc2_R45 ,val.adc2_LDR);
    delay_ms(500);
  }
}



















