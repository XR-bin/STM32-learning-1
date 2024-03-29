#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    uint8_t data;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化  115200 */

    while(1)
    {
        data = USART1_Receive_Byte();
        USART1_Send_Byte(data);
    }
}




