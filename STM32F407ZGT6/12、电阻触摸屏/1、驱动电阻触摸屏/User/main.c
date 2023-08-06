#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"

int main(void)
{
    uint16_t x=0,y=0;

    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);      /* 串口1初始化 115200 */
    Touch_IO_Init();          /* 电阻触摸屏引脚初始化 */

    while(1)
    {
        if(T_PEN == 0)
        {
            x = Touch_Data(TOUCH_X);          /* 获取触点x坐标 */
            y = Touch_Data(TOUCH_Y);          /* 获取触点y坐标 */
            printf("x:%4d   y:%4d\r\n",x,y);
        }
    }
}




