#include "sys.h"
#include "delay.h"
#include "timer.h"
#include "pwm.h"

int main(void)
{
    sys_stm32_clock_init(9);         /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);     /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                  /* 延时初始化 */
    TIM6_Init(500,7200);             /* 500ms 定时中断 */
    TIM3_PWM_Init(1000, 72, 500);    /* 50%占空比 */

    while(1)
    {
    }
}




