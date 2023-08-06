#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    sys_stm32_clock_init(9);      /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);  /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();               /* 延时初始化 */
    LED_Init();                   /* LED初始化 */
    EXTI4_Init();                 /* 外部中断4初始化 */

    while(1)
    {
    }
}




