#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

int main(void)
{
//  BEEP_Init();
  LED_Init();
//  BEEP_ON;
  
  while(1)
  {
    //LED_Flash2(5);
    LED_All_ON();
    delay_ms(500);
    LED_All_OFF();
    delay_ms(500);
  }
  
}


