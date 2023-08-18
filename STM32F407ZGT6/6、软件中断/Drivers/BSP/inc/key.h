#ifndef __KEY_H
#define __KEY_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    #define KEY0    ((GPIOE->IDR>>4) & 0x1)         /* 低电平按下 */
    #define KEY1    ((GPIOE->IDR>>3) & 0x1)         /* 低电平按下 */
    #define KEY2    ((GPIOE->IDR>>2) & 0x1)         /* 低电平按下 */
    #define KEY_UP  ((GPIOA->IDR>>0) & 0x1)         /* 高电平按下 */



    /****************    函数外部声明   *****************/
    void KEY_Init(void);         /* 按键初始化 */
    uint8_t KEY_Scan(void);      /* 按键扫描函数 */

#endif




