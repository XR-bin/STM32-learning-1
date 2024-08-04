#ifndef __SHOW_H
#define __SHOW_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void LCD_Show_HZ16(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop);  /* 显示一个16*16字符/汉字 */
    void LCD_Show_HZ32(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop);  /* 显示一个32*32字符/汉字 */

#endif
