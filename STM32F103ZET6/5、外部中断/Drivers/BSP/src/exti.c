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
* @funcName ：EXTI4_Init
* @brief    ：外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void EXTI4_Init(void)
{
    uint32_t pri;

    /* GPIOx初始化设置 */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= (1<<6);

    /* 端口配置寄存器 */
    GPIOE->CRL &= ~(0xf<<(4*4));
    GPIOE->CRL |= (8<<(4*4));

    /* 端口输出寄存器  设置高电平：正式上拉 设置低电平：正式下拉 */
    GPIOE->ODR |= (1<<4);

    /* 外部中断配置 */
    /* AFIO时钟使能 */
    RCC->APB2ENR |= (1<<0);

    /* 外部中断配置寄存器 */
    AFIO->EXTICR[1] &= ~(0xf<<0);
    AFIO->EXTICR[1] |= (4<<0);

    /* 上升沿触发选择寄存器 */
    EXTI->RTSR |= (1<<4);

    /*中断分组*/
    /* 优先级分组（主函数里写） */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(EXTI4_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(EXTI4_IRQn);

    /* 中断屏蔽寄存器 */
    EXTI->IMR |= (1<<4);
}



/**********************************************************
* @funcName ：EXTI4_IRQHandler
* @brief    ：外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0--------PE4 低电平按下，高电平抬起
* @fn       ：
************************************************************/
void EXTI4_IRQHandler(void)
{
    if(EXTI->PR & (1<<4))
    {
        delay_ms(20);
        if((GPIOE->IDR & (1<<4)) == 0)
        {
            GPIOB->ODR ^= (1<<5);     /* LED0翻转 */
            EXTI->PR |= (1<<4);       /* 写1清除中断标志 */
        }
    }
}












