#ifndef __IWDG_H
#define __IWDG_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void IWDG_Init(uint8_t prer, uint16_t rlr);      /* 独立看门狗初始化 */
    void IWDG_Feed(void);                            /* 喂狗 */

#endif






