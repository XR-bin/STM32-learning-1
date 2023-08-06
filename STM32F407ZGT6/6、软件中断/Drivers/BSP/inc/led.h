#ifndef _LED_H
#define _LED_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* 开灯宏定义 */
    #define LED1_ON GPIOF->ODR &= ~(1<<9)
    #define LED2_ON GPIOF->ODR &= ~(1<<10)
    /* 关灯宏定义 */
    #define LED1_OFF GPIOF->ODR |= (1<<9)
    #define LED2_OFF GPIOF->ODR |= (1<<10)



    /****************    函数外部声明   *****************/
    void LED_Init(void);         /* LED初始化 */

#endif








