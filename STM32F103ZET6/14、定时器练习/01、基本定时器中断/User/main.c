#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    sys_stm32_clock_init(9);      /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);  /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();               /* 延时初始化 */
    LED_Init();                   /* LED初始化 */
    TIM6_Init(4999, 7200);        /* 定时器6初始化  500ms定时 */

    while(1)
    {
        LED0_ON;
        delay_ms(500);
        LED0_OFF;
        delay_ms(500);
    }
}




