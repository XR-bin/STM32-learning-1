#include "stm32f1xx.h"
#include "delay.h"
#include "led.h"
#include "beep.h"

int main(void)
{
    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    LED_Init();               /* LED驱动初始化 */
    BEEP_Init();              /* BEEP驱动初始化 */

    while(1)
    {
        LED0_ON;
        LED1_ON;
        BEEP_ON;
        delay_ms(600);
        LED0_OFF;
        LED1_OFF;
        BEEP_OFF;
        delay_ms(600);
    }
}




