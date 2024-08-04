#include "adc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\adc.c
*              .\Drivers\BSP\inc\adc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：ADC测芯片内部温度代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/*****************************************
* F407的ADC时钟频率官方建议: 不超过36Mhz
* F103的ADC时钟频率官方建议: 不超过14Mhz
******************************************/



/**********************************************************
* @funcName ：ADC1_In16_Init
* @brief    ：ADC1的第16个通道初始化配置
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            无需引脚，用于测芯片温度
************************************************************/
void ADC1_In16_Init(void)
{
    /* ADCx寄存器配置 */
    /* ADCx时钟使能 */
    RCC->APB2ENR |= 1<<8;                 /* ADC1 */

    /* ADCx控制寄存器1   CR1 */
    /* 独立模式 */
    /* 禁止规则通道的不连续采样模式 */
    /* 禁止扫描模式 */
    ADC1->CR1 = 0;

    /* ADCx控制寄存器2   CR2 */
    /* 右对齐 */
    /* 单次转换模式 */
    ADC1->CR2 = 0;
    ADC1->CR2 |= 1<<10;

    /* ADCx采样时间寄存 */
    ADC1->SMPR1 &= ~(7<<18);
    ADC1->SMPR1 |= (7<<18);      /* ADCx采样时间寄存  采样时间为239.5个周期 */

    /* ADCx规则序列寄存器 */
    ADC1->SQR1 &= ~(0xf<<20);    /* 通道数目  1个 */
    ADC1->SQR3 &= ~(0x1f<<0);
    ADC1->SQR3 |= (16<<0);       /* 第1个转换的通道是通道6 */

    /* ADC通用控制寄存器 */
    ADC->CCR &= ~(3<<16);
    ADC->CCR |= (1<<16);         /* 4分频 84/4=21Mhz */
    ADC->CCR |= 1<<23;           /* 温度传感器使能 */

    /* ADCx使能 */
    ADC1->CR2 |= (1<<0);
}



/**********************************************************
* @funcName ：ADC1_In16_Data
* @brief    ：获取ADC1通道16的转换数据
* @param    ：void
* @retval   ：uint16_t
* @details  ：
* @fn       ：
*            检测芯片温度
************************************************************/
uint16_t ADC1_In16_Data(void)
{
    uint16_t data;

    //开启规则通道转换开关
    ADC1->CR2 |= (1<<30);
    //等待转换完成
    while(!(ADC1->SR & (1<<1)));
    //读取转换后的数据
    data = ADC1->DR;

    return data;
}



/**********************************************************
* @funcName ：Oversampling
* @brief    ：ADC过采样算法，提高分辨率，由12分辨率变为16分辨率
* @param    ：void
* @retval   ：uint16_t  ---  (ADC的16分辨率数据)
* @details  ：
* @fn       ：
*            用于转换光敏电阻数据
************************************************************/
uint16_t Oversampling(void)
{
    int i;
    uint32_t sum=0;

    for(i=0; i<256; i++)
    {
        sum+=ADC1_In16_Data();
    }

    return (sum>>4);
}



/**********************************************************
* @funcName ：ADC1_Chip_Temperature1
* @brief    ：将ADC值转换为芯片温度值(非过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature1(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = ADC1_In16_Data();
        stm_t[i]=(float)data*(3.3/4096);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



/**********************************************************
* @funcName ：ADC1_Chip_Temperature2
* @brief    ：将ADC值转换为芯片温度值(过采样)
* @param    ：void
* @retval   ：double
* @details  ：
* @fn       ：
*            用于检测芯片温度
************************************************************/
double ADC1_Chip_Temperature2(void)
{
    uint8_t i;
    uint32_t data;
    double sum = 0;
    double stm_t[6];

    for(i=0;i<5;i++)
    {
        data = Oversampling();
        stm_t[i]=(float)data*(3.3/65536);     /* 电压值 */
        stm_t[i]=(1.43-stm_t[i])/0.0043+25;  /* 转换为温度值 */
        sum += stm_t[i];
    }

    stm_t[5] = sum/5;

    return stm_t[5];
}



