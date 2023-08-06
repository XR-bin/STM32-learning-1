#include "stm32f1xx.h"
#include "delay.h"
#include "led.h"
#include "key.h"

int main(void)
{
    uint8_t key = 0;
    uint8_t flay = 1;
    uint8_t i = 0;
    uint32_t cont = 0;

    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    LED_Init();               /* LED初始化 */
    KEY_Init();               /* KEY初始化 */

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




