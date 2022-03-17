#include "sys.h"		
#include "delay.h"	
#include "led.h"
#include "beep.h"
#include "key.h"

int main(void)
{		
  u8 key = 0;
  u8 flay = 1;
  u8 i = 0;
  u32 cont = 0;
  
	delay_init(72);	  	//—” ±≥ı ºªØ
  LED_Init();
  KEY_Init();
  
	while(1)
	{
    key = KEY_Scan();
    switch(key)
    {
      case 1:LED0_ON;LED1_OFF;flay = 0;break;
      case 2:LED1_ON;LED0_OFF;flay = 0;break;
      case 3:flay = 1;LED0_OFF;LED1_OFF;break;
    }
    if(flay)
    {
      if(i)
      {
        LED0_ON;
        LED1_OFF;
      }
      else 
      {
        LED0_OFF;
        LED1_ON;
      }
      cont++;
      if(cont ==60000)
      {
        i=!i;
        cont = 0;
      }
    }
	}	 
}




