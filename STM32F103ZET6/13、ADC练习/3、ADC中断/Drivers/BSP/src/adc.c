#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC中断练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
****************************************/



/**********************************************************
* @funcName ：ADC3_In6_Init
* @brief    ：ADC3的第6个通道初始化配置(带ADC中断)
* @param    ：void
* @retval   ：void
* @details  ：
*             PF8----------ADC3_IN6   模拟模式
* @fn       ：
************************************************************/
void ADC3_In6_Init(void)
{
    uint32_t pri;

    /* GPIOx寄存器配置 */
    RCC->APB2ENR |= (1<<7);     /* GPIOx时钟使能 */
    GPIOF->CRH &= ~(0xf<<0);    /* 端口模式寄存器(模拟模式) */

    /* ADCx相关时钟配置 */
    RCC->APB2ENR |= (1<<15);      /* ADCx时钟使能 */
    RCC->CFGR &= ~(3<<14);        /* ADC时钟预分频 6分频  72M/6=12M */
    RCC->CFGR |= (2<<14);

    ADC3->CR1 &= ~(0xffffffff);   /* ADCx控制寄存器1   CR1 */
    ADC3->CR2 &= ~(0xffffffff);   /* ADCx控制寄存器2   CR2 */
    ADC3->CR2|=7<<17;             /* 软件外部触发 */
    ADC3->CR2|=1<<20;             /* 使用外部事件启动转换 */

    ADC3->SMPR2 &= ~(7<<(3*6));   /* ADCx采样时间寄存  采样时间为239.5个周期 */
    ADC3->SMPR2 |= (7<<(3*6));

    /* ADCx规则序列寄存器 */
    ADC3->SQR1 &= ~(0xf<<20);     /* 通道数目  1个 */
    ADC3->SQR3 &= ~(0x1f<<0);     /* 第1个转换的通道是通道6 */
    ADC3->SQR3 |= (6<<0);

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(ADC3_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(ADC3_IRQn);

    /* 使能中断 */
    ADC3->CR1 |= (1<<5);

    /* ADCx使能 */
    ADC3->CR2 |= (1<<0);  

    /*校准部分*/ 
    ADC3->CR2|=1<<3;        /* 使能复位校准 */
    while(ADC1->CR2&1<<3);  /* 等待校准结束 */
    /* 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 */
    ADC3->CR2|=1<<2;        /* 开启AD校准 */
    while(ADC3->CR2&1<<2);  /* 等待校准结束 */
    /* 该位由软件设置以开始校准，并在校准结束时由硬件清除 */
}



/**********************************************************
* @funcName ：ADC3_IRQHandler
* @brief    ：ADC3中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void ADC3_IRQHandler(void)
{
    uint16_t data;
    if(ADC3->SR & (1<<1))
    { 
        /* 读取转换后的数据 */
        data = ADC3->DR;
        printf("data:%d\r\n",data);
    }
}





