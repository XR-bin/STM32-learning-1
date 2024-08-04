#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    uint16_t data1=0;
    uint16_t data2=0;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化 115200 */
    ADC3_In5_Init();                        /* ADC3通道5初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        data1=0, data2=0;
        USART1_Receive_Byte();
        data1 = ADC3_In5_Data();
        data2 = Oversampling();

        printf("单次采样：%d\r\n", data1);
        printf("过采样：%d\r\n", data2);
    }
}




