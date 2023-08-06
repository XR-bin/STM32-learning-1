#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\w25q128.c
*              .\Drivers\BSP\inc\w25q128.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：w25q128读写操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/***
*W25Q128存储芯片
*容量大小             ：16Mbyte == 128Mbit
*共有块区             ：256块
*每块区共有扇区       ：16扇区
*每扇区共有页         ：16页
*每页字节             ：256字节
*地址可写最大十六进制数：0x9fffff
*        9f:块区   f：扇区    f：页    ff：字节
*/



/**********************************************************
* @funcName ：W25Q128_Init
* @brief    ：W25Q128初始化
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void NorFlash_Init(void)
{
    /* 片选引脚 */
    /* GPIOx时钟 */
    RCC->AHB1ENR |= (1<<1);
    /* 端口模式寄存器PB14 */
    GPIOB->MODER &= ~(3<<2*14);
    GPIOB->MODER |= (1<<2*14);         /* 通用输出 */
    /* 端口输出类型寄存器 */
    GPIOB->OTYPER &= ~(1<<14);         /* 推挽 */
    //端口输出速度寄存器
    GPIOB->OSPEEDR &= ~(3<<2*14);
    GPIOB->OSPEEDR |= (2<<2*14);
    //端口输出数据寄存器(片选拉高)
    GPIOB->ODR |= (1<<14);

    SPI1_Init();                       /* SPI初始化 */
}



/**********************************************************
* @funcName ：NorFlash_Read_ID
* @brief    ：读取norflash芯片的ID
* @param    ：void
* @retval   ：uint16_t -- 返回芯片ID
* @details  ：
* @fn       ：
************************************************************/
uint16_t NorFlash_Read_ID(void)
{
    uint16_t id;

    SPI_CS_L;                               /* 片选使能 */
    SPI1_RS_Byte(FLASH_ManufactDeviceID);   /* 发送读 ID 命令 */
    SPI1_RS_Byte(0);                        /* 写入一个字节 */
    SPI1_RS_Byte(0);
    SPI1_RS_Byte(0);
    id =  SPI1_RS_Byte(0xFF) << 8;          /* 读取高8位字节 */
    id |= SPI1_RS_Byte(0xFF);               /* 读取低8位字节 */
    SPI_CS_H;                               /* 片选失能 */

    /* 芯片ID列表参考norfalsh.h里的宏定义 */
    printf("芯片ID : %x\r\n", id);

    return id;
}



