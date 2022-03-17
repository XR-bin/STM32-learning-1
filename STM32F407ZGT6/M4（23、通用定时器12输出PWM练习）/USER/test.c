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
  u8 key;
  
  SysTick_Init();
  USART1_Init(115200);
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
  KEY_Init();
  LCD_Init();
  TIM12_PWM_Init(1000,84,500);
  
  while(1)
  {
    key = KEY_Scan();
    if(key == 3)
    {
      if(TIM12->CCR2 == 0)
      {
        TIM12->CCR2 = 0;
      }
      else 
      TIM12->CCR2 -= 100;
    }
    else if(key == 4)
    {
      if(TIM12->CCR2 == 1000)
      {
        TIM12->CCR2 = 1000;
      }
      else 
      TIM12->CCR2 += 100;
    }
  }
}



















