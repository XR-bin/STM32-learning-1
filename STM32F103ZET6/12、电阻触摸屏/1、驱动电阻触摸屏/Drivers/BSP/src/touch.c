#include "touch.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-03
* @brief     ：电阻触摸屏驱动配置和操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：Touch_IO_Init
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            CS     PF11  推挽输出
*            SCK    PB1   推挽输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   推挽输出
* @fn       ：
************************************************************/
void Touch_IO_Init(void)
{
    /* PB */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= 1<<3;
    /* 端口配置寄存器 */
    GPIOB->CRL &= ~((0xf<<(4*1))|(0xf<<(4*2)));
    GPIOB->CRL |= ((3<<(4*1))|(4<<(4*2)));

    /* PF */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= 1<<7;
    /* 端口配置寄存器 */
    GPIOF->CRH &= ~((0xf<<(4*(9-8)))|(0xf<<(4*(10-8)))|(0xf<<(4*(11-8))));
    GPIOF->CRH |= ((3<<(4*(9-8)))|(4<<(4*(10-8)))|(3<<(4*(11-8))));

    /* 初始化状态空闲 */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ：Touch_Data
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：uint8_t command (指令)
* @retval   ：uint16_t
* @details  ：
*            片选线        T_CS_H       T_CS_L  
*            时钟线        T_SCK_H      T_SCK_L  
*            输出数据线    T_MOSI_H     T_MOSI_L
*            输入数据线    T_MISO
*            笔接触线      T_PEN
* @fn       ：
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;            /* 数据线拉低 */
    T_CS_L;              /* 片选使能 */

    for(i=0;i<8;i++)
    {
        T_SCK_L;             /* 时钟线拉低 */

        /* 判断指令位电平 */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;  /* 指令数据左移 */

        T_SCK_H;          /* 时钟线拉高 */
        delay_us(1);      /* 延时，用于从机读取数据 */
    }
    T_SCK_L;            /* 时钟线拉低 */
    T_MOSI_L;           /* 数据线拉低 */
    delay_us(6);        /* 延时6us，用于等待ADC把数据转换好 */

    /* 空出一个空脉冲 */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;              /* 指令数据左移 */
        T_SCK_L;                 /* 时钟线拉低 */
        delay_us(1);             /* 延时，用于数据建立 */
        T_SCK_H;                 /* 时钟线拉高 */
        if(T_MISO) data |= 0x1;  /* 读取数据位电平 */
        delay_us(1);             /* 延时，用于数据建立 */
    }

    T_CS_H;    /* 片选拉高 */
    T_SCK_L;   /* 时钟线拉低 */

    return data;
}





