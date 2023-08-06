#include "sys.h"
#include "delay.h"
#include "key.h"
#include "pwm.h"

int main(void)
{
    uint8_t key;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    KEY_Init();                             /* 按键初始化 */
    TIM14_PWM_Init(1000, 72, 500);          /* 50%占空比 */

    while(1)
    {
        key = KEY_Scan();
        if(key == 1)
        {
            if(TIM14->CCR1 == 0) TIM14->CCR1 = 0;
            else                TIM14->CCR1 -= 100;
        }
        else if(key == 2)
        {
            if(TIM14->CCR1 == 1000) TIM14->CCR1 = 1000;
            else                   TIM14->CCR1 += 100;
        }
    }
}




