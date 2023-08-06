#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();                             /* LED初始化 */
    EXTI0_Init();                           /* 外部中断0初始化 */

    while(1)
    {
    }
}




