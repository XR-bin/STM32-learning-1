#include "sys.h"
#include "delay.h"
#include "key.h"
#include "pwm.h"

int main(void)
{
    uint8_t key;

    sys_stm32_clock_init(9);          /* 系统时钟初始化*/
    SysTick_Init();                   /* 延时初始化 */
    KEY_Init();                       /* 按键初始化 */
    TIM3_PWM_Init(1000, 72, 500);     /* 50%占空比 */

    while(1)
    {
        key = KEY_Scan();
        if(key == 1)
        {
            if(TIM3->CCR2 == 0) TIM3->CCR2 = 0;
            else                TIM3->CCR2 -= 100;
        }
        else if(key == 2)
        {
            if(TIM3->CCR2 == 1000) TIM3->CCR2 = 1000;
            else                   TIM3->CCR2 += 100;
        }
    }
}




