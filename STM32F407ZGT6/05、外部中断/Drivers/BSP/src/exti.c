#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：外部中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI0_Init
* @brief    ：外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY_UP--------PA0 高电平按下，低电平抬起
* @fn       ：
************************************************************/
void EXTI0_Init(void)
{
    uint32_t pri;

    /* GPIOx初始化设置 */
    /* GPIOx时钟使能 */
    RCC->AHB1ENR |= (1<<0);

    /* 端口模式寄存器 */
    GPIOA->MODER &= ~(3<<0);     /* 输入模式 */
    /* 端口上下拉 */
    GPIOA->PUPDR &= ~(3<<0);     /* 下拉 */
    /* 端口输出数据寄存器 */
    GPIOA->ODR &= ~(1<<0);

    /* EXTI控制器配置 */
    /* 系统配置控制器时钟使能 */
    RCC->APB2ENR |= (1<<14);
    /* 映射管脚 */
    SYSCFG->EXTICR[0] &= ~(0xf<<0);
    /* 上升沿/下降沿寄存器配置 */
    EXTI->RTSR &= ~(1<<0);
    EXTI->RTSR |= (1<<0);        /* 上升沿触发 */

    /*中断分组*/
    /* 优先级分组（主函数里写） */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(EXTI0_IRQn, pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(EXTI0_IRQn);

    /* 中断屏蔽器 */
    EXTI->IMR |= (1<<0);
}



/**********************************************************
* @funcName ：EXTI0_IRQHandler
* @brief    ：外部中断0服务函数
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY_UP--------PA0 高电平按下，低电平抬起
* @fn       ：
************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & (1<<0))
    {
        delay_ms(20);
        if((GPIOA->IDR & (1<<0)) == 0)
        {
            GPIOF->ODR ^= (1<<9);     /* LED0翻转 */
            EXTI->PR |= (1<<0);       /* 写1清除中断标志 */
        }
    }
}


