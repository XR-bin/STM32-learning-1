#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    LED_Init();               /* LED初始化 */

    while(1)
    {
        LED0_ON;
        LED1_OFF;
        delay_ms(500);
        LED0_OFF;
        LED1_ON;
        delay_ms(500);
    }
}




