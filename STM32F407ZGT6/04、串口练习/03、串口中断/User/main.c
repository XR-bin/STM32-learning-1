#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    NVIC_SetPriorityGrouping(5);            /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化   115200 */

    while(1)
    {
    }
}




