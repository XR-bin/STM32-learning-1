#include "led.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\led.c
*              .\Drivers\BSP\inc\led.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：LED灯驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：LED_Init
* @brief    ：对LED对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            LED0     PB5
*            LED1     PE5
*            高电平灭，低电平亮-----输出模式
* @fn       ：
************************************************************/
void LED_Init(void)
{
    /* 端口时钟使能 */
    RCC->APB2ENR |= (1<<3);   /* GPIOB时钟 */
    RCC->APB2ENR |= (1<<6);   /* GPIOE时钟 */

    /* 端口模式配置 */
    /* 端口输出类型配置 */
    /* 端口输出速度配置 */
    GPIOB->CRL &= ~(0xF<<4*5);
    GPIOB->CRL |= (3<<4*5);

    GPIOE->CRL &= ~(0xF<<4*5);
    GPIOE->CRL |= (3<<4*5);

    /* 端口输出数据寄存器配置     LED灯的初始状态 */
    GPIOB->ODR |= (1<<5);
    GPIOE->ODR |= (1<<5);
}













