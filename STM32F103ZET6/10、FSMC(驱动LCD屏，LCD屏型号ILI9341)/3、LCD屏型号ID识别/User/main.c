#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"

int main(void)
{
    sys_stm32_clock_init(9);  /* 系统时钟初始化*/
    SysTick_Init();           /* 延时初始化 */
    USART1_Init(115200);      /* 串口1初始化   115200 */
    LCD_ILI9341_Init();       /* ILI9341的LCD屏初始化 */

    delay_ms(50);

    /* 能识别的型号：ST7789、ILI9341、NT35310、NT35510、SSD1963 */
    LCD_Recognition_Id();

    while(1)
    {
    }
}




