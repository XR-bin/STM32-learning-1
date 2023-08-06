#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main(void)
{
    uint8_t key_flay;

    sys_stm32_clock_init(9);      /* 系统时钟初始化*/
    NVIC_SetPriorityGrouping(5);  /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();               /* 延时初始化 */
    LED_Init();                   /* LED初始化 */
    KEY_Init();                   /* KEY初始化 */
    EXTI5_Swier_Init();           /* 软件外部中断初始化 */

    while(1)
    {
        key_flay = KEY_Scan();
        if(key_flay == 2)
        {
            EXTI->SWIER |= (1<<5);     /* 触发软件中断 */
        }
    }
}




