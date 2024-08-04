#ifndef __CAPTURE_H
#define __CAPTURE_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "usart.h"



    /****************    函数外部声明   *****************/
    void TIM4_Capture_Init(uint16_t arr, uint16_t psc);   /* 对通用定时器4进行初始化设置并捕获脉冲信号 */

#endif
