#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "iwdg.h"

int main(void)
{
    uint8_t key=0;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();               /* LED初始化 */
    KEY_Init();               /* KEY初始化 */
    IWDG_Init(4, 1500);       /* 预分频数为64,重载值为1500,溢出时间为3s */

    delay_ms(1000);
    LED0_ON;

    while(1)
    {
        key = KEY_Scan();

        if(key) 
        {
            IWDG_Feed();
        }
    }
}




