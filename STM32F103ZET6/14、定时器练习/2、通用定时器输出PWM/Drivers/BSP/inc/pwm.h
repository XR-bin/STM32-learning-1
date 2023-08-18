#ifndef __PWM_H
#define __PWM_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr);   /* 对通用定时器3进行初始化设置并输出PWM */

#endif
