#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();                             /* LED驱动初始化 */
    BEEP_Init();                            /* BEEP驱动初始化 */

    while(1)
    {
    }
}




