#include "dma.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\dma.c
*              .\Drivers\BSP\inc\dma.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：DMA练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：DMA1_Usart1_Init
* @brief    ：对DMA1的通道4进行初始化设置(连接USART1的发送通道)
* @param    ：uint8_t* data (DMA存储器地址)
* @retval   ：void
* @details  ：
* @fn       ：
*            DMA1将存储器的数据搬运到串口1外设
************************************************************/
void DMA1_Usart1_Init(uint8_t* data)
{
    RCC->AHBENR |= 1<<0;               /* 开启DMA1时钟 */

    delay_ms(5);                       /* 等待DMA时钟稳定 */

    DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;  /* DMA 外设地址 */
    DMA1_Channel4->CMAR = (uint32_t)data;         /* DMA 存储器地址 */
    DMA1_Channel4->CNDTR = 0;                     /* DMA 传输长度清零, 后续在dma_enable函数设置 */


    /* DMA通道x配置寄存器 */
    DMA1_Channel4->CCR = 0;            /* 复位CCR寄存器 */
    DMA1_Channel4->CCR &= ~(1<<14);    /* 非存储器到存储器模式 */
    DMA1_Channel4->CCR &= ~(3<<12);    /* 通道优先级   低 */
    DMA1_Channel4->CCR &= ~(3<<10);    /* 存储器数据宽度  8位 */
    DMA1_Channel4->CCR &= ~(3<<8);     /* 外设数据宽度    8位 */
    DMA1_Channel4->CCR &= ~(1<<6);     /* 不执行外设地址增量模式 */
    DMA1_Channel4->CCR &= ~(1<<5);     /* 非循环模式(即使用普通模式) */
    DMA1_Channel4->CCR |= (1<<7);      /* 执行存储器地址增量操作 */
    DMA1_Channel4->CCR |= (1<<4);      /* 数据传输方向 寄存器到外设 */

    /* 使能串口DMA发送 */
    USART1->CR3 |= 1<<7;
}



/**********************************************************
* @funcName ：DMA1_Enable
* @brief    ：开启DMA1数据传输
* @param    ：uint16_t cndtr (DMA传输数据量)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void DMA1_Enable(uint16_t cndtr)
{
    DMA1_Channel4->CCR &= ~(1 << 0);         /* 关闭DMA传输 */

    while (DMA1_Channel4->CCR & (1 << 0));   /* 确保DMA可以被设置 */

    DMA1_Channel4->CNDTR = cndtr;            /* DMA传输数据量 */
    DMA1_Channel4->CCR |= 1 << 0;            /* 开启DMA传输 */
}










