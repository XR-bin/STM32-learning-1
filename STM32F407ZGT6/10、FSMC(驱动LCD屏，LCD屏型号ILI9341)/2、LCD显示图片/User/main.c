#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "tu1.h"
#include "tu2.h"

/**
* 注意：这个程序案例用了大量的SRAM空间，如果烧录F1型号的STM32后卡死，可能
*       是SRAM溢出了，这里我用的是STM32F103ZET6的大容量STM32，能够成功运行。
*/

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LCD_ILI9341_Init();                     /* ILI9341的LCD屏初始化 */

    /**
    * 注意：tu1和tu2的图片格式是不是一样的
    *       tu1的颜色数据是低位在前，高位在后，例如 uint16的颜色数据为 0xF001，转为uint8数组成员 0x01 0xF0
    *       tu2的颜色数据是高位在前，低位在后，例如 uint16的颜色数据为 0xF001，转为uint8数组成员 0xF0 0x01
    *
    * LCD_Picture1和LCD_Picture3用来处理tu1格式的，LCD_Picture2是用来处理tu2格式的
    * LCD_Picture1和LCD_Picture3的实现方式不一样
    */
    LCD_Picture1(0, 0, (uint8_t*)gImage_tu1);
    delay_ms(8000);
    LCD_Picture2(0, 0, (uint8_t*)gImage_tu2);
    delay_ms(8000);
    LCD_Picture3(0, 0, (uint8_t*)gImage_tu1);

    while(1)
    {
    }
}




