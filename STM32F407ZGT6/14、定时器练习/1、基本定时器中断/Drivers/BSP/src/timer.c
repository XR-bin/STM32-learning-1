#include "timer.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\timer.c
*              .\Drivers\BSP\inc\timer.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：基本定时器中断练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：TIM6_Init
* @brief    ：对定时器6进行初始化设置
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_Init(uint16_t arr, uint16_t psc)
{
    uint32_t pri;

    /* TIMx时钟使能 */
    RCC->APB1ENR |= 1<<4;

    /* 控制寄存器CR1 */
    TIM6->CR1 &= ~(0xffff);
    TIM6->CR1 |= 1<<7;                  /* 自动重装载预装载使能 */

    TIM6->PSC = psc;                    /* 预分频器 */
    TIM6->ARR = arr;                    /* 自动重装载寄存器 */

    TIM6->EGR |= 1<<0;                  /* 更新事件生成寄存器(人为产生一个更新) */
    TIM6->SR &= ~(1<0);                 /* 清除更新事件标志 */

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(TIM6_DAC_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    /* 中断寄存器 */
    TIM6->DIER &= ~(0xffff);
    TIM6->DIER |= 1<<0;

    /* 开计数器 */
    TIM6->CR1 |= (1<<0);
}



/**********************************************************
* @funcName ：TIM6_DAC_IRQn
* @brief    ：定时器6的中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM6_DAC_IRQHandler(void)
{
    /* 判断是否定时器6中断 */
    if(TIM6->SR & (1<<0))
    {
        /* 清中断标志位 */
        TIM6->SR &= ~(1<<0);
        /* 紧急事件 */
        GPIOF->ODR ^= (1<<10);     /* LED1闪烁 */
    }
}



