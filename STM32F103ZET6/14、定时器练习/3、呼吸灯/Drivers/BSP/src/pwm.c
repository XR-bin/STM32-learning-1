#include "pwm.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\pwm.c
*              .\Drivers\BSP\inc\pwm.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-27
* @brief     ：PWM练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/




/**********************************************************
* @funcName ：TIM3_PWM_Init
* @brief    ：对通用定时器3进行初始化设置并输出PWM
* @param    ：uint16_t arr (重载值)
* @param    ：uint16_t psc (预分频值)
* @param    ：uint16_t ccr (比较值/占空比)
* @retval   ：void
* @details  ：
*            PWM输出口  PB5   TIM3_CH2  输出通道2
* @fn       ：
*            有效电平为高电平
*            定时器向上计数
*            先输出有效电平后输出无效电平
************************************************************/
void TIM3_PWM_Init(uint16_t arr, uint16_t psc, uint16_t ccr)
{
    /* GPIOx配置部分 */
    RCC->APB2ENR |= 1<<3;             /* GPIOB时钟使能 */
    /* 端口配置寄存器 */
    GPIOB->CRL &= ~(0xf<<(4*5));
    GPIOB->CRL |= (0xb<<(4*5));       /* 复用推挽输出 */
    /* 复用重映射寄存器 */
    RCC->APB2ENR |= 1<<0;             /* 辅助IO时钟使能 */
    AFIO->MAPR &= ~(3<<10);
    AFIO->MAPR |= (2<<10);            /* 定时器3通道2映射到PB5 */

    /* 定时器设置部分 */
    RCC->APB1ENR |= 1<<1;             /* 开启TIM3时钟 */
    /* 控制寄存器1  CR1 */
    TIM3->CR1 &= ~(0xffff);
    TIM3->CR1 |= 1<<7;                /* 自动重装载预装载使能 */
    /* 从模式控制寄存器(选择内部时钟源) */
    TIM3->SMCR &= ~(7<<0);
    /* 预分频器 */
    TIM3->PSC = psc;
    /* 自动重载寄存器 */
    TIM3->ARR = arr;
    /* 捕获/比较模式寄存器1 */
    TIM3->CCMR1 &= ~(0xff00);
    TIM3->CCMR1 |= 0x6800;        /* 开启预装载功能，向上计数时先输出有效，向下计数时先输出无效 */
    //捕获/比较使能寄存器
    TIM3->CCER &= ~(0xf0);        /* 高电平为有效电平 */
    TIM3->CCER |= 1<<4;           /* 使能输出 */
    /* 捕获/比较寄存器2 */
    TIM3->CCR2 = ccr;             /* 设置比较值 */
    /* 事件生成寄存器 */
    TIM3->EGR |= 1<<0;            /* 触发更新 */
    /* 清除事件生成标志 */
    TIM3->SR &= ~(1<0);

    /* 计算器使能 */
    TIM3->CR1 |= 1<<0;
}
