#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "w25q128.h"

int main(void)
{
    uint8_t str[] = "asd123456789zxc";
    uint8_t arr[30] = {0};

    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    USART1_Init(115200);      /* 串口初始化  115200 */
    W25Q128_Init();           /* W25Q128初始化 */

    W25Q128_Write(0x00, sizeof(str), str);
    W25Q128_Read_Bytes(0x00, sizeof(str), arr);

    printf("str：%s\r\n", str);
    printf("arr：%s\r\n", arr);

    while(1)
    {
    }
}




