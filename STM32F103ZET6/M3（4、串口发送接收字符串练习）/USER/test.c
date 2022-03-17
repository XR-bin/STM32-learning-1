#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h" 

#define SEND

int main(void)
{				 
  u8 str[20];
  
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);	  	//延时初始化
  LED_Init();
  BEEP_Init();
  KEY_Init();
  USART1_Init(9600);
  USART2_Init(9600);
  
  LED1_ON;
  LED0_ON;
  
	while(1)
	{
    USART1_Receive_Str(str);
    printf("%s\r\n",str);                                                                                                                                                                                                               
	}	 
}




