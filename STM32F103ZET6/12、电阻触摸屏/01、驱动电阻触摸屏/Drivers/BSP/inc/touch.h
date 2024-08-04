#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 片选线 */
    #define T_CS_H    GPIOF->ODR |= (1<<11)      /* 片选失能 */
    #define T_CS_L    GPIOF->ODR &= ~(1<<11)     /* 片选使能 */
    /* 时钟线 */
    #define T_SCK_H   GPIOB->ODR |= (1<<1)
    #define T_SCK_L   GPIOB->ODR &= ~(1<<1)
    /* 输出线 */
    #define T_MOSI_H  GPIOF->ODR |= (1<<9)
    #define T_MOSI_L  GPIOF->ODR &= ~(1<<9)
    /* 输入线 */
    #define T_MISO    (GPIOB->IDR & (1<<2))
    /* 笔接触线 */
    #define T_PEN     (GPIOF->IDR >> 10 & 0x1)

    /* 指令 */
    #define TOUCH_X   0xD0    //获取x坐标数据
    #define TOUCH_Y   0x90    //获取y坐标数据



    /****************    函数外部声明   *****************/
    void Touch_IO_Init(void);                /* 电阻触摸屏引脚初始化 */
    uint16_t Touch_Data(uint8_t command);    /* 向电阻触摸屏发送数据并接收电阻触摸屏发来的数据 */

#endif







