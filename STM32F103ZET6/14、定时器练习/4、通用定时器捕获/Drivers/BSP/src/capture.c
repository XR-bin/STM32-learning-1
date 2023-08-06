#include "capture.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\capture.c
*              .\Drivers\BSP\inc\capture.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：捕获练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：TIM4_Capture_Init
* @brief    ：对通用定时器4进行初始化设置并捕获脉冲信号
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @retval   ：void
* @details  ：
*            脉冲输入口  PB6   TIM4_CH1  输入通道1  映射通道IC1、IC2
* @fn       ：
************************************************************/
void TIM4_Capture_Init(uint16_t arr, uint16_t psc)
{
    uint32_t pri;

    /* GPIOx配置部分 */
    RCC->APB2ENR |= 1<<3;             /* GPIOB时钟使能 */
    /* 端口配置寄存器 */
    GPIOB->CRL &= ~(0xf<<(4*6));
    GPIOB->CRL |= (0x8<<(4*6));       /* 上下拉输入模式 */
    GPIOB->ODR |= 0<<6;               /* PB6 下拉模式 */
    //复用重映射寄存器
    RCC->APB2ENR |= 1<<0;             /* 辅助IO时钟使能 */
    AFIO->MAPR &= ~(1<<12);           /* 定时器4通道1映射到PB6 */

    /*TIMx配置*/
    RCC->APB1ENR |= 1<<2;             /* 开启TIM4时钟 */
    /* 控制寄存器1  CR1 */
    TIM4->CR1 &= ~(0xffff);
    TIM4->CR1 |= 1<<7;                /* 自动重装载预装载使能 */
    /* 从模式控制寄存器 */
    TIM4->SMCR &= ~(7<<4);
    TIM4->SMCR |= (5<<4);              /* 滤波后的定时器输入 1 (TI1FP1) */
    TIM4->SMCR &= ~(7<<0);
    TIM4->SMCR |= (4<<0);              /* 复位模式 */

    /*********ICC1通道**********/
    /* 捕获比较模式寄存器1 */
    TIM4->CCMR1 &= ~(0xf<<4);
    TIM4->CCMR1 |= 0x3<<4;     /* 翻转。当TIMx_CCR1=TIMx_CNT时，翻转OC1REF的电平 */
    TIM4->CCMR1 &= ~(3<<0);
    TIM4->CCMR1 |= 1<<0;       /* 通道CH1进来的信号用通道IC1去捕获 */
    /* 捕获比较使能寄存器 */
    TIM4->CCER &= ~(1<<1);     /* 上升沿捕获，使能捕获 */
    TIM4->CCER |= (1<<0);      /* 使能捕获 */

    /*********ICC2通道**********/
    //捕获比较模式寄存器1
    TIM4->CCMR1 &= ~(0xf<<12);
    TIM4->CCMR1 |= 0x3<<12;      /* 翻转。当TIMx_CCR1=TIMx_CNT时，翻转OC1REF的电平 */
    TIM4->CCMR1 &= ~(3<<8);
    TIM4->CCMR1 |= 2<<8;         /* 通道CH1进来的信号用通道IC2去捕获 */
    //捕获比较使能寄存器
    TIM4->CCER |= (1<<5);        /* 下降沿捕获 */
    TIM4->CCER |= (1<<4);        /* 使能捕获 */

    //中断还能寄存器
    TIM4->DIER |= (1<<1);        /* 使能 CC1 中断 */
    TIM4->DIER |= (1<<2);        /* 使能 CC2 中断 */

    /* 预分频器 */
    TIM4->PSC = psc;
    /* 自动重装载寄存器 */
    TIM4->ARR = arr;
    /* 事件生成寄存器 */
    TIM4->EGR |= 1<<0;
    /* 清除事件生成标志 */
    TIM4->SR &= ~(1<0);

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(TIM4_IRQn, pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(TIM4_IRQn);

    /* TIMx使能 */
    TIM4->CR1 |= 1<<0;
}



/**********************************************************
* @funcName ：TIM4_IRQHandler
* @brief    ：定时器4中断函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void TIM4_IRQHandler(void)
{
    static uint16_t data_2;
    uint16_t data_1;
    uint16_t p_val;

    if(TIM4->SR & (1<<2))
    {
        /* 清除捕通道2的获标志位 */
        TIM4->SR &= ~(1 << 2);
        /* 紧急事件 */
        /* CCR2 */
        data_2 = TIM4->CCR2;
    }

    if(TIM4->SR & (1<<9))
    {
        /* 清除通道1重复捕获标志位 */
        TIM4->SR &= ~(1 << 9);
        /* 清除通道1的捕获标志位 */
        TIM4->SR &= ~(1 << 1);
        /* 紧急事件 */
        /* 记录值CCR1 */
        data_1 = TIM4->CCR1;
        /* 计算周期和占空比 */
        p_val = (float)data_2/data_1*100;

        printf("周期：%d   高电平：%d   占空比：%d%%\r\n",data_1,data_2,p_val);
    }
}

