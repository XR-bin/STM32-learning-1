#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

int main(void)
{
    sys_stm32_clock_init(9);              /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);          /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                       /* 延时初始化 */
    USART1_Init(115200);                  /* 串口1初始化 115200 */
    RTC_Set_Alarm(2023, 8, 1, 12, 0, 1);  /* 设置闹钟 */
    RTC_Init();                           /* 初始化RTC */

    while(1)
    {
    }
}




