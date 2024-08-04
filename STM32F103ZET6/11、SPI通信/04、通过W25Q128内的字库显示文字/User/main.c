#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "w25q128.h"
#include "show.h"

/***
* 注意：使用这个程序前，必须保证已经烧录了字库
*/

int main(void)
{
    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    USART1_Init(115200);      /* 串口初始化  115200 */
    LCD_ILI9341_Init();       /* ILI9341的LCD屏初始化 */
    W25Q128_Init();           /* W25Q128初始化 */

    LCD_Show_HZ16(100,100,(uint8_t*)"你好啊hahaha123",RED,WHITE);
    LCD_Show_HZ32(10,200,(uint8_t*)"你好啊haha123",RED,WHITE);

    while(1)
    {
    }
}




