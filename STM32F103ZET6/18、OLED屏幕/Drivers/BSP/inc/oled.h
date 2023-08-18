#ifndef __OLED_H
#define __OLED_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 数据和命令的选择宏定义 */
    #define  OLED_CMD     0      /* 命令 */
    #define  OLED_DATA    1      /* 数据 */

    /* OLED屏大小 */
    #define  OLED_LINE       128    /* 128列像素 */
    #define  OLED_ROW        8      /* 8*8行像素(1页8行，共有8页) */



    /****************    函数外部声明   *****************/
    uint8_t OLED_Write_Command(uint8_t cmd);                  /* 对OLE屏发送指令 */
    uint8_t OLED_Write_Data(uint8_t data);                    /* 对OLE屏发送数据 */
    void OLED_WR_Byte(uint8_t Byte, uint8_t DC);              /* 对OLE屏发送指令/数据 */
    void OLED_Clear(void);                                    /* 对OLE屏清屏 */
    void OLED_XY(uint8_t x,uint8_t y);                        /* 设置显示光标的坐标 */
    void OLED_Open(void);                                     /* 开启OLED屏 */
    void OLED_Close(void);                                    /* 关闭OLED屏 */
    void OLED_Init(void);                                     /* OLED屏初始化 */
    void OLED_ShowChar16(uint16_t x,uint8_t y, uint8_t ch);   /* 显示一个16*16字符 */
    void OLED_ShowString16(uint8_t x,uint8_t y,uint8_t *str); /* 显示一个16*16字符串 */

#endif
