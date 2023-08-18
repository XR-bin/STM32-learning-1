#include "sys.h"
#include "delay.h"
#include "lcd.h"

int main(void)
{
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    LCD_ILI9341_Init();                     /* ILI9341的LCD屏初始化 */

//    LCD_Draw_Circle(100, 100, 50, RED);           /* 空心圆 */
//    LCD_Fill_Circle(100, 100, 50, BLUE);          /* 实心圆 */
//    LCD_Draw_Rectangle(50, 50, 100, 100, BLACK);  /* 空心矩形 */
//    LCD_Fill(50, 50, 100, 100, YELLOW);           /* 实心矩形 */
//    LCD_ILI9341_ClearXY(50, 50, 100, 100, GREEN); /* 实心矩形 */
//    LCD_Draw_Line(50, 50, 100, 100, MAGENTA);     /* 画斜线 */
//    LCD_Draw_Hline(50, 50, 100, MAGENTA);         /* 水平直线 */

//    LCD_String16(50, 50, "123asd,./", RED);       /* 显示16*16字符串 */
//    LCD_String24(50, 80, "123asd,./", BLUE);      /* 显示24*24字符串 */
//    LCD_String32(50, 120, "123asd,./", GREEN);    /* 显示32*32字符串 */

    /* 注意：这个事例代码的汉字字模我就取了这三个字 */
    LCD_HanziStr32(50, 120, "方思虹", DARKBLUE);    /* 显示32*32汉字 */

    while(1)
    {
    }
}




