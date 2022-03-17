#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h" 
#include "nvic.h"

int main(void)
{				 
	Stm32_Clock_Init(9);//系统时钟设置
  NVIC_SetPriorityGrouping(5);  //7 - 抢占优先级二进制位数
	delay_init(72);	  	//延时初始化
  LED_Init();
  BEEP_Init();
  KEY_Init();
  USART1_Init(9600);
  
	while(1)
	{
    LED_Flash1();                                                                                                                                                                                                        
	}	 
}




