#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC配置代码
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
* @funcName ：ADC3_In5_Init
* @brief    ：ADC3的第5个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
*             PF7----------ADC3_IN5   模拟模式
* @fn       ：
************************************************************/
void ADC3_In5_Init(void)
{
    uint32_t pri;

    /* GPIOx寄存器配置 */
    /* GPIOx时钟使能 */
    RCC->AHB1ENR |= (1<<5);           /* GPIOF */
    //端口模式寄存器(模拟模式)
    GPIOF->MODER &= ~(3<<(2*7));
    GPIOF->MODER |= (3<<(2*7));       /* 模拟输出 */

    /* 复位ADC */
    RCC->APB2RSTR |= 1 << 8;          /* ADC1 & ADC2 & ADC3 复位, 注意, 这里复位了所有ADC!!! */
    RCC->APB2RSTR &= ~(1 << 8);       /* 复位结束 */
    /* ADCx寄存器配置 */
    /* ADCx时钟使能 */
    RCC->APB2ENR |= 1<<10;            /* ADC3 */
    ADC->CCR &= ~(3<<16);             /* ADCPRE[1:0] ADC时钟预分频清零 */
    ADC->CCR |= 1<<16;                /* 设置ADC时钟预分频系数为 4, 即 PCLK2 / 4 = 21Mhz */

    //ADCx控制寄存器1   CR1
    ADC3->CR1 = 0;                    /* CR1清零 */
    ADC3->CR1 |= 0<<8;                /* 非扫描模式 */
    ADC3->CR1 |= 0<<24;               /* 12位模式 */
    //ADCx控制寄存器2   CR2
    ADC3->CR2 = 0;                    /* CR2清零 */
    ADC3->CR2 |= 0 << 1;              /* 单次转换模式 */
    ADC3->CR2 |= 0 << 11;             /* 右对齐 */
    ADC3->CR2 |= 0 << 28;             /* 软件触发 */

    ADC3->SMPR2 &= ~(7<<(3*6));
    ADC3->SMPR2 |= (7<<(3*6));        /* ADCx采样时间寄存  采样时间为239.5个周期 */

    /* ADCx规则序列寄存器 */
    ADC3->SQR1 &= ~(0xf<<20);         /* 通道数目  1个 */
    ADC3->SQR3 &= ~(0x1f<<0);
    ADC3->SQR3 |= (5<<0);             /* 第1个转换的通道是通道5 */

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(ADC_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(ADC_IRQn);

    /* 使能中断 */
    ADC3->CR1 |= (1<<5);
    /* ADCx使能 */
    ADC3->CR2 |= (1<<0);

    /* 开启规则通道转换开关 */
    ADC3->CR2 |= (1<<30);
}



/**********************************************************
* @funcName ：ADC_IRQHandler
* @brief    ：ADC中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void ADC_IRQHandler(void)
{
    uint16_t data;
    if(ADC3->SR & (1<<1))
    { 
        /* 读取转换后的数据 */
        data = ADC3->DR;
        printf("data:%d\r\n",data);
    }
}









