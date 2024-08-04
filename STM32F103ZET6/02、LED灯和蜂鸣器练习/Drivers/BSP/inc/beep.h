#ifndef _BEEP_H
#define _BEEP_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    #define BEEP_ON  GPIOB->ODR |= (1<<8)        /* 开蜂鸣器 */
    #define BEEP_OFF GPIOB->ODR &= ~(1<<8)       /* 关蜂鸣器 */



    /****************    函数外部声明   *****************/
    void BEEP_Init(void);       /* BEEP初始化 */

#endif










