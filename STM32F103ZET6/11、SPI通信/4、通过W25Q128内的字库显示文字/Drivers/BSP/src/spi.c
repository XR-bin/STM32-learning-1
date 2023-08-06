#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\spi.c
*              .\Drivers\BSP\inc\spi.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：SPI配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：SPI2_Init
* @brief    ：对SPI2通信要用的GPIO口进行初始化和对SPI2的寄存器进行设置
* @param    ：void
* @retval   ：void
* @details  ：
*            PB13   SPI2_SCK   时钟线  复用推挽
*            PB14   SPI2_MISO  主输入  复用推挽
*            PB15   SPI2_MOSI  主输出  复用推挽
*            极性：SCK为低电平为空闲状态
*            相位：数据采样从第一个时钟边沿开始
* @fn       ：
************************************************************/
void SPI2_Init(void)
{
    /* GPIOB时钟使能 */
    RCC->APB2ENR |= 1<<3;
    //端口配置寄存器
    GPIOB->CRH &= ~(0xfff00000);
    GPIOB->CRH |= 0xbbb00000;
    /* 复用重映射寄存器 */
    /* 端口输出寄存器输出高 */
    GPIOB->ODR|=0X7<<13; 

    /* SPI2寄存器初始化设置 */
    /* SPI2时钟使能 */
    RCC->APB1ENR |= 1<<14;
    /* SPI2控制寄存器1 */
    SPI2->CR1 &= ~(0xffff);
    SPI2->CR1 |= (1<<9);     /* 启用软件从设备管理 */
    SPI2->CR1 |= (1<<8);     /* 进入主模式 */
    SPI2->CR1 |= (0<<3);     /* 波特率：fPCLK/2  ==  36M/2  ==  18M */
    SPI2->CR1 |= (1<<2);     /* 配置为主设备 */
    /* SPI2控制寄存器2 */
    SPI2->CR2 &= ~(0xff);
    /* SPI_I2S配置寄存器 */
    SPI2->I2SCFGR &= ~(0xfff);
    /* 使能SPI2 */
    SPI2->CR1|=1<<6;
}



/**********************************************************
* @funcName ：SPI2_RS_Byte
* @brief    ：SPI2接收/发送一个字节数据(8位)
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
uint8_t SPI2_RS_Byte(uint8_t data)
{
    /* 判断发送缓存区是否为空 */
    while(!(SPI2->SR & (1<<1)));
    SPI2->DR = data;
    /* 判断接收缓存区是否为空 */
    while(!(SPI2->SR & (1<<0)));
    data = SPI2->DR;

    return data;
}






