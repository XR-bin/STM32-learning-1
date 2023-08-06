#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "norflash.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口初始化  115200 */
    NorFlash_Init();                        /* W25Q128初始化 */

    NorFlash_Read_ID();

    while(1)
    {
    }
}




