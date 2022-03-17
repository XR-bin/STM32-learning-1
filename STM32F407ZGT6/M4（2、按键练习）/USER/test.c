#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"

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
  u8 key = 0;
  u8 key2 = 0;
  u8 flay = 0;
  u8 sleep = 5;
  
  LED_Init();
  KEY_Init();
  
  while(1)
  {
    key = KEY_Scan();
    key2 = KEY_Scan2();
    switch(key)
    {
      case 0 : break;
      case 1 : flay = 1; break;
      case 2 : flay = 0; LED_All_OFF(); break;
      case 3 : sleep++; if(sleep >= 10)sleep=10; break;
      case 4 : sleep--; if(sleep <= 1)sleep=1; break;
    }
    if(flay)LED_Flash3(sleep);
    if(key2==12)
    {
      flay = 0;
      LED_All_ON();
    }
  }
}


