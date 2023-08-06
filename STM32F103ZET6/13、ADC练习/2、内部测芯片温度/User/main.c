#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    double data1=0;
    double data2=0;

    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    USART1_Init(115200);      /* 串口1初始化 115200 */
    ADC1_In16_Init();         /* ADC1通道16初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        data1=0, data2=0;
        USART1_Receive_Byte();
        data1 = ADC1_Chip_Temperature1();
        data2 = ADC1_Chip_Temperature2();

        printf("单次采样：%f\r\n", data1);
        printf("过采样：%f\r\n", data2);
    }
}




