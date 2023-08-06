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
* @brief    ：对DMA2的通道7进行初始化设置(连接USART1的发送通道)
* @param    ：uint8_t* data (DMA存储器地址)
* @retval   ：void
* @details  ：
* @fn       ：
*            DMA2将存储器的数据搬运到串口1外设
************************************************************/
void DMA2_Usart1_Init(uint8_t ch, uint8_t* data)
{
    RCC->AHB1ENR |= 1<<22;             /* 开启DMA2时钟 */

    delay_ms(5);                       /* 等待DMA时钟稳定 */

    DMA2_Stream7->CR = (ch & 7) << 25;           /* 数据流通道选择, 0 ~ 7 */
    DMA2_Stream7->PAR = (uint32_t)&USART1->DR;   /* DMA 外设地址 */
    DMA2_Stream7->M0AR = (uint32_t)data;         /* DMA 存储器地址 */
    DMA2_Stream7->NDTR = 0;                      /* DMA 传输长度清零, 后续在dma_enable函数设置 */

    /* DMA通道x配置寄存器 */
    DMA2_Stream7->CR = 0;           /* 复位CCR寄存器 */
    DMA2_Stream7->CR |= 0 << 8;      /* 非循环模式(即使用普通模式) */
    DMA2_Stream7->CR |= 0 << 9;      /* 外设非增量模式 */
    DMA2_Stream7->CR |= 0 << 11;     /* 外设数据长度:8位 */
    DMA2_Stream7->CR |= 0 << 13;     /* 存储器数据长度:8位 */
    DMA2_Stream7->CR |= 0 << 21;     /* 外设突发单次传输 */
    DMA2_Stream7->CR |= 0 << 23;     /* 存储器突发单次传输 */
    DMA2_Stream7->CR |= 1 << 6;      /* 存储器到外设模式 */
    DMA2_Stream7->CR |= 1 << 10;     /* 存储器增量模式 */
    DMA2_Stream7->CR |= 1 << 16;     /* 中等优先级 */

    /* 使能串口DMA发送 */
    USART1->CR3 |= (1<<7);
}



/**********************************************************
* @funcName ：DMA2_Enable
* @brief    ：开启DMA2数据传输
* @param    ：uint16_t cndtr (DMA传输数据量)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void DMA2_Enable(uint16_t ndtr)
{
    DMA2_Stream7->CR &= ~(1 << 0);   /* 关闭DMA传输 */

    while (DMA2_Stream7->CR & 0X1);  /* 确保DMA可以被设置 */

    DMA2_Stream7->NDTR = ndtr;       /* 要传输的数据项数目 */
    DMA2_Stream7->CR |= 1 << 0;      /* 开启DMA传输 */
}





