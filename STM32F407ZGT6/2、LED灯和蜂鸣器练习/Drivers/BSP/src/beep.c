#include "beep.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\beep.c
*              .\Drivers\BSP\inc\beep.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：BEEP蜂鸣器驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：BEEP_Init
* @brief    ：对BEEP对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            BEEP     PF8
*            高电平响，低电平不响-----输出模式
* @fn       ：
************************************************************/
void BEEP_Init(void)
{
    /* 端口时钟使能  GPIOF */
    RCC->AHB1ENR |= (1<<5);

    /* 端口模式配置 */
    GPIOF->MODER &= ~(3<<(2*8));
    GPIOF->MODER |= (1<<(2*8));        /* 通用输出 */

    //端口输出类型配置
    GPIOF->OTYPER &= ~(1<<8);          /* 推挽输出 */

    /* 端口输出速度配置 */
    GPIOF->OSPEEDR &= ~(3<<(2*8));
    GPIOF->OSPEEDR |= (2<<(2*8));      /* 速度为50MHz */

    /* 端口输出数据寄存器配置 */
    /* 先将蜂鸣器关闭 */
    GPIOF->ODR &= ~(1<<8);             /* 低电不响 */
}


