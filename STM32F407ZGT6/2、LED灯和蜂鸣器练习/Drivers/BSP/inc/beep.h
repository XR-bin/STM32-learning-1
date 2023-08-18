#ifndef __BEEP_H
#define __BEEP_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    #define BEEP_ON  GPIOF->ODR |= (1<<8)      /* 开蜂鸣器 */
    #define BEEP_OFF GPIOF->ODR &= ~(1<<8)     /* 关蜂鸣器 */



    /****************    函数外部声明   *****************/
    void BEEP_Init(void);       /* BEEP初始化 */

#endif

