#ifndef __LED_H
#define __LED_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* 开灯 */
    #define LED0_ON GPIOF->ODR &= ~(1<<9)
    #define LED1_ON GPIOF->ODR &= ~(1<<10)
    /* 关灯 */
    #define LED0_OFF GPIOF->ODR |= (1<<9)
    #define LED1_OFF GPIOF->ODR |= (1<<10)



    /****************    函数外部声明   *****************/
    void LED_Init(void);         /* LED初始化 */

#endif








