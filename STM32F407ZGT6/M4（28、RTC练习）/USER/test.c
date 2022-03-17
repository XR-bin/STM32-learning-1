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
#include "time.h"
#include "pwm.h"
#include "dma.h"
#include "rtc.h"

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
*         2、AT24C02的0~16用于存储LCD与触摸屏之间的校准系数
***************************************************************/ 

int main(void)
{   
  RTC_DATES time = {19,10,10,6,12,20,20,0};
  RTC_DATES rtc;
  SysTick_Init();
  USART1_Init(115200);
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
  RTC_Init(time);
  while(1)
  {
    RTC_Read_Time(&rtc);
    RTC_Read_date(&rtc);
    printf("20%d年%d月%d日 星期%d\r\n",rtc.year,rtc.month,rtc.day,rtc.week);
    printf("%d : %d : %d\r\n",rtc.hour,rtc.min,rtc.sec);
    
    delay_ms(1000);
  }
}



















