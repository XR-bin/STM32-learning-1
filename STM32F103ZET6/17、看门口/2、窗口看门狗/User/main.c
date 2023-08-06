#include "sys.h"
#include "delay.h"
#include "led.h"
#include "wwdg.h"

int main(void)
{
    sys_stm32_clock_init(9);      /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);  /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();               /* 延时初始化 */
    LED_Init();                   /* LED初始化 */
    LED0_ON;                      /* LED0亮 */
    delay_ms(300);                /* 持续300ms */
    WWDG_Init(0x7f, 0x5f, 3);     /* 计数器值为7f,窗口寄存器为5f,分频数为8 */

    while(1)
    {
        LED0_OFF;             /* LED0灭 */
    }
}




