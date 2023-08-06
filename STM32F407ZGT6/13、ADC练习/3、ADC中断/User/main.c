#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化 115200 */
    ADC3_In5_Init();                        /* ADC3通道6初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        USART1_Receive_Byte();
        /* 开启规则通道转换开关 */
        ADC3->CR2 |= (1<<22);
    }
}




