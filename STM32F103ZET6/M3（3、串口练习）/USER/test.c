#include "sys.h"		
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"

int main(void)
{		
  u8 data;
  
  Stm32_Clock_Init(9);	//系统时钟设置
  USART1_Init(9600);
	while(1)
	{
    data = USART1_Receive_Byte();
    USART1_Send_Byte(data);
  }
}




