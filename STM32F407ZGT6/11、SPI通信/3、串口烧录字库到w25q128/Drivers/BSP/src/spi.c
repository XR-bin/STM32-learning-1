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
* @funcName ：SPI1_Init
* @brief    ：对SPI1通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
* @param    ：void
* @retval   ：void
* @details  ：
*            PB3   SPI2_SCK   时钟线  复用推挽
*            PB4   SPI2_MISO  主输入  复用推挽
*            PB5   SPI2_MOSI  主输出  复用推挽
*            极性：SCK为低电平为空闲状态
*            相位：数据采样从第一个时钟边沿开始
* @fn       ：
************************************************************/
void SPI1_Init(void)
{
    /* GPIOx配置部分 */
    /* GPIOx时钟使能 */
    RCC->AHB1ENR |= (1<<1);
    /* 端口模式寄存器 */
    GPIOB->MODER &= ~(0x3f<<6);
    GPIOB->MODER |= ((2<<(2*3))|(2<<(2*4))|(2<<(2*5)));    /* 复用输出 */
    /* 端口输出类型寄存器 */
    GPIOB->OTYPER &= ~(1<<3);
    GPIOB->OTYPER &= ~(1<<5);                              /* 推挽 */
    /* 端口输出速率寄存器 */
    GPIOB->OSPEEDR &= ~(3<<2*3);
    GPIOB->OSPEEDR &= ~(3<<2*5);
    GPIOB->OSPEEDR |= (2<<2*3);
    GPIOB->OSPEEDR |= (2<<2*5);
    /* 端口输入上拉/下拉寄存器 */
    GPIOB->PUPDR &= ~(3<<2*4);                             /* 无上下拉 */
    /* 端口复用功能寄存器 */
    GPIOB->AFR[0] &= ~(0xfff<<4*3);
    GPIOB->AFR[0] |= (0x555<<4*3);

    /* 配置SPI寄存器 */
    /* SPI1时钟使能 */
    RCC->APB2ENR |= (1<<12);
    /* 控制寄存器CR1 */
    SPI1->CR1 &= ~(0xffff);
    SPI1->CR1 |= (1<<9);       /* 启用软件从设备管理 */
    SPI1->CR1 |= (1<<8);       /* 进入主模式 */
    SPI1->CR1 |= (1<<3);       /* 波特率：fPCLK/2  ==  84M/4  ==  21M */
    SPI1->CR1 |= (1<<2);       /* 配置为主设备 */
    /* 控制寄存器CR2 */
    SPI1->CR2 &= ~(0xff);
    /* 配置寄存器 */
    SPI1->I2SCFGR &= ~(0xfff);

    /* 使能SPI1 */
    SPI1->CR1 |= 1<<6;
}



/**********************************************************
* @funcName ：SPI1_RS_Byte
* @brief    ：SPI1接收/发送一个字节数据(8位)
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
uint8_t SPI1_RS_Byte(uint8_t data)
{
    /* 判断发送缓存区是否为空 */
    while(!(SPI1->SR & (1<<1)));
    SPI1->DR = data;
    /* 判断接收缓存区是否为空 */
    while(!(SPI1->SR & (1<<0)));
    data = SPI1->DR;

    return data;
}

