#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"

int main(void)
{
    uint8_t key = 0;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();                             /* LED初始化 */
    KEY_Init();                             /* KEY初始化 */

    while(1)
    {
        key = KEY_Scan();
        switch(key)
        {
            case 1:LED0_ON;LED1_OFF;break;
            case 2:LED1_ON;LED0_OFF;break;
            case 3:LED0_ON;LED1_ON;break;
            case 4:LED0_OFF;LED1_OFF;break;
        }
    }
}




