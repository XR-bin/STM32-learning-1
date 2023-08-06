#ifndef _LED_H
#define _LED_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    #define LED0_ON  GPIOB->ODR &= ~(1<<5)
    #define LED1_ON  GPIOE->ODR &= ~(1<<5)
    #define LED0_OFF GPIOB->ODR |= (1<<5)
    #define LED1_OFF GPIOE->ODR |= (1<<5)



    /****************    函数外部声明   *****************/
    void LED_Init(void);     /* LED初始化 */ 

#endif









