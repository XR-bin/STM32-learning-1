#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"

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
  u8 data;
  
  USART1_Init(9600);
  while(1)
  {
    data = USART1_Receive_Byte();
    USART1_Send_Byte(data);
  }
}


