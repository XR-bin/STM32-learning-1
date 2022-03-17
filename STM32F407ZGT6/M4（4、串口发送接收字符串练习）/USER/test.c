#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"

#define RECEIVE

//static void delay_ms(u32 ms)
//{
//	u32 i = 168 / 4 * 1000 * ms;
//	while(i)
//	{
//		i--;
//	}
//}

int main(void)
{  
  u8 data[20];
  
  USART1_Init(9600);
//  USART2_Init(9600);
  LED_Init();
  KEY_Init();   
  
  while(1)
  {
    USART1_Receive_Str(data);
//    USART1_Send_Str(data);
    printf("%s\r\n",data);
  }
  
}











