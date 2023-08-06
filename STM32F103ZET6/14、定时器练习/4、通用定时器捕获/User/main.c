#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "capture.h"

int main(void)
{
    sys_stm32_clock_init(9);       /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);   /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                /* 延时初始化 */
    USART1_Init(115200);           /* 串口1初始化 115200 */
    TIM3_PWM_Init(1000, 72, 500);  /* 输出PWM，50%占空比 */
    TIM4_Capture_Init(0xffff, 72); /* 捕获初始化 */
    /***
    * 注意：因为捕获中断用了重复捕获标记中断，
    * 所以TIM4_Capture_Init必须放在TIM3_PWM_Init之后
    */

    while(1)
    {
    }
}




