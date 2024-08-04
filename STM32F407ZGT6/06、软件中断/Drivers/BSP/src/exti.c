#include "exti.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：软件中断代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：EXTI5_Swier_Init
* @brief    ：软件外部中断初始化
* @param    ：void
* @retval   ：void
* @details  ：
*            无需端口
* @fn       ：
************************************************************/
void EXTI5_Swier_Init(void)
{
    uint32_t pri;

    /* 外部中断配置 */
    /* 软件中断事件寄存器 */
    EXTI->SWIER &= ~(1<<5);

    /* 中断分组 */
    /* 优先级分组（主函数里写）*/
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(EXTI9_5_IRQn, pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(EXTI9_5_IRQn); 

    /* 中断屏蔽寄存器 */
    EXTI->IMR |= (1<<5);
}



/**********************************************************
* @funcName ：EXTI9_5_IRQHandler
* @brief    ：外部中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if(EXTI->PR & (1<<5))
    {
        GPIOF->ODR ^= (1<<9);            /* LED0翻转 */
        EXTI->PR |= (1<<5);              /* 写1清除中断标志 */
    }
}

