#include "w25q128.h"
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
void W25Q128_Init(void)
{
    /* 片选引脚 */
    /* GPIOx时钟 */
    RCC->APB2ENR |= 1<<3;
    /* 端口配置寄存器 */
    GPIOB->CRH &= ~(0xf<<(4*(12-8)));
    GPIOB->CRH |= (0x3<<(4*(12-8)));
    /* 端口输出数据寄存器(片选拉高) */
    GPIOB->ODR |= 1<<12;

    SPI2_Init();         /* SPI初始化 */
}

 
/**********************************************************
* @funcName ：W25Q128_Read_SR
* @brief    ：读取25QXX的状态寄存器，25QXX一共有3个状态寄存器
* @param    ：uint8_t serial      状态寄存器序列号，范围:1~3
*   @arg    ：1 --- 状态寄存器1   2 --- 状态寄存器2   3 --- 状态寄存器3
* @retval   ：uint8_t (状态寄存器值)
* @details  ：
* @fn       ：
************************************************************/
uint8_t W25Q128_Read_SR(uint8_t serial)
{
    uint8_t byte = 0, command = 0;

    switch (serial)
    {
        /* 读状态寄存器1指令 */
        case 1: command = FLASH_ReadStatusReg1; break;
        /* 读状态寄存器2指令 */
        case 2: command = FLASH_ReadStatusReg2; break;
        /* 读状态寄存器3指令 */
        case 3: command = FLASH_ReadStatusReg3; break;
        /* serial无法匹配时 */
        default: command = FLASH_ReadStatusReg1; break;
    }

    SPI_CS_L;                   /* 使能片选 */
    SPI2_RS_Byte(command);      /* 发送读寄存器命令 */
    byte = SPI2_RS_Byte(0Xff);  /* 读取一个字节 */
    SPI_CS_H;                   /* 失能片选 */

    return byte;
}



/**********************************************************
* @funcName ：W25Q128_Write_SR
* @brief    ：写25QXX的状态寄存器，25QXX一共有3个状态寄存器
* @param    ：uint8_t serial (状态寄存器序列号，范围:1~3)
*   @arg    ：1 --- 状态寄存器1   2 --- 状态寄存器2   3 --- 状态寄存器3
* @param    ：uint8_t data (要写入状态寄存器的数据)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Write_SR(uint8_t serial, uint8_t data)
{
    uint8_t command = 0;

    switch (serial)
    {
        /* 写状态寄存器1指令 */
        case 1: command = FLASH_WriteStatusReg1; break;
        /* 写状态寄存器2指令 */
        case 2: command = FLASH_WriteStatusReg2; break;
        /* 写状态寄存器3指令 */
        case 3: command = FLASH_WriteStatusReg3; break;
        /* serial无法匹配时 */
        default: command = FLASH_WriteStatusReg1; break;
    }

    SPI_CS_L;               /* 使能片选 */
    SPI2_RS_Byte(command);  /* 发送读寄存器命令 */
    SPI2_RS_Byte(data);     /* 写入一个字节 */
    SPI_CS_H;               /* 失能片选 */
}



/**********************************************************
* @funcName ：W25Q128_Wait_Busy
* @brief    ：等待空闲
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
static void W25Q128_Wait_Busy(void)
{
    while ((W25Q128_Read_SR(1) & 0x01) == 0x01);   /* 等待BUSY位清空 */
}



/**********************************************************
* @funcName ：W25Q128_Write_Enable
* @brief    ：对W25Q128进行写使能
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Write_Enable(void)
{
    SPI_CS_L;
    SPI2_RS_Byte(FLASH_WriteEnable);
    SPI_CS_H;
}



/**********************************************************
* @funcName ：W25Q128_Send_Address
* @brief    ：向W25QXX发送地址
* @param    ：uint32_t address (w25Q128内部的存储地址)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
static void W25Q128_Send_Address(uint32_t address)
{
    SPI2_RS_Byte((uint8_t)((address)>>16));     /* 发送 bit23 ~ bit16 地址 */
    SPI2_RS_Byte((uint8_t)((address)>>8));      /* 发送 bit15 ~ bit8  地址 */
    SPI2_RS_Byte((uint8_t)address);             /* 发送 bit7  ~ bit0  地址 */
}



/**********************************************************
* @funcName ：W25Q128_Read_Bytes
* @brief    ：主控芯片从W25Q128的某个地址读n个字节数据
* @param    ：uint32_t address  (w25Q128内部的存储地址)
* @param    ：uint32_t datalen  (要读取的数据字节数)
* @param    ：uint8_t *arr      (存放数据的缓存数组)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Read_Bytes(uint32_t address, uint32_t datalen, uint8_t *arr)
{
    SPI_CS_L;                       /* 片选使能 */
    SPI2_RS_Byte(FLASH_ReadData);   /* 发读数据指令 */
    W25Q128_Send_Address(address);  /* 发送地址 */

    /* 收数据 */
    while(datalen)
    {
        *arr = SPI2_RS_Byte(0xff);
        arr++;
        datalen--;
    }

    SPI_CS_H;      /* 片选失能 */
}



/**********************************************************
* @funcName ：W25Q128_Page_Write
* @brief    ：主控芯片从W25Q128的某个地址连写n个字节数据(不可跨页)
* @param    ：uint32_t address  (w25Q128内部的存储地址)
* @param    ：uint32_t datalen  (要读取的数据字节数)
* @param    ：uint8_t *arr      (存放要写入的数据的数组)
* @retval   ：void
* @details  ：
*            注意：
*                开始写入的地址最大32bit
*                要写入的字节数最大256,该数不应该超过该页的剩余字节数!!!
* @fn       ：
************************************************************/
void W25Q128_Page_Write(uint32_t address, uint32_t datalen, uint8_t *arr)
{
    W25Q128_Write_Enable();    /* 写使能 */

    SPI_CS_L;                        /* 片选使能 */
    SPI2_RS_Byte(FLASH_PageProgram); /* 写页指令 */
    W25Q128_Send_Address(address);   /* 发送地址 */

    while(datalen)
    {
        SPI2_RS_Byte(*arr);
        arr++;
        datalen--;
    }

    SPI_CS_H;      /* 片选失能 */
    W25Q128_Wait_Busy();   /* 等待写完过程 */
}



/**********************************************************
* @funcName ：W25Q128_Write_NoCheck
* @brief    ：无检验写SPI FLASH
* @param    ：uint32_t address  (w25Q128内部的存储地址)
* @param    ：uint16_t datalen  (要读取的数据字节数)
* @param    ：uint8_t *arr      (存放要写入的数据的数组)
* @retval   ：void
* @details  ：
* @fn       ：
*           注意：必须确保所写的地址范围内的数据全部为0XFF,
*                 否则在非0XFF处写入的数据将失败!
*
*            该函数具有自动换页功能
*
*            在指定地址开始写入指定长度的数据,但是要确保地址不越界!
************************************************************/
static void W25Q128_Write_NoCheck(uint32_t address, uint16_t datalen, uint8_t *arr)
{
    uint16_t pageremain;
    pageremain = 256 - address % 256;  /* 单页剩余的字节数 */

    if (datalen <= pageremain)         /* 不大于256个字节 */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* 当写入字节比页内剩余地址还少的时候, 一次性写完
         * 当写入直接比页内剩余地址还多的时候, 先写完整个页内剩余地址, 然后根据剩余长度进行不同处理
         */
        W25Q128_Page_Write(address, pageremain, arr);

        if (datalen == pageremain)   /* 写入结束了 */
        {
            break;
        }
        else                            /* datalen > pageremain */
        {
            arr += pageremain;          /* arr指针地址偏移,前面已经写了pageremain字节 */
            address += pageremain;      /* 写地址偏移,前面已经写了pageremain字节 */
            datalen -= pageremain;      /* 写入总长度减去已经写入了的字节数 */

            if (datalen > 256)          /* 剩余数据还大于一页,可以一次写一页 */
            {
                pageremain = 256;       /* 一次可以写入256个字节 */
            }
            else                        /* 剩余数据小于一页,可以一次写完 */
            {
                pageremain = datalen;   /* 不够256个字节了 */
            }
        }
    }
}



uint8_t g_norflash_buf[4096];   /* 扇区缓存，先把W25Q128的某个扇区读到这个缓存内，再擦扇区 */
/**********************************************************
* @funcName ：W25Q128_Write
* @brief    ：在指定地址开始写入指定长度的数据 , 该函数带擦除操作!
* @param    ：uint32_t address  (w25Q128内部的存储地址，最大32bit)
* @param    ：uint16_t datalen  (要读取的数据字节数)
* @param    ：uint8_t *arr      (存放要写入的数据的数组，最大65535)
* @retval   ：void
* @details  ：
*             SPI FLASH 一般是: 256个字节为一个Page, 
*                               4Kbytes为一个Sector, 
*                               16个扇区为1个Block
*              擦除的最小单位为Sector.
* @fn       ：
************************************************************/
void W25Q128_Write(uint32_t address, uint16_t datalen, uint8_t *arr)
{
    uint32_t secpos;       /* 扇区地址 */
    uint16_t secoff;       /* 扇区内地址 */
    uint16_t secremain;    /* 当前商区剩余大小 */
    uint16_t i;
    uint8_t *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = address / 4096;       /* 扇区地址 */
    secoff = address % 4096;       /* 在扇区内的偏移 */
    secremain = 4096 - secoff;     /* 扇区剩余空间大小 */

    //printf("ad:%X,nb:%X\r\n", address, datalen); /* 测试用 */

    if (datalen <= secremain)
    {
        secremain = datalen;    /* 不大于4096个字节 */
    }

    while (1)
    {
        W25Q128_Read_Bytes(secpos * 4096, 4096, norflash_buf);  /* 读出整个扇区的内容 */

        for (i = 0; i < secremain; i++)   /* 校验数据 */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* 需要擦除, 直接退出for循环 */
            }
        }

        if (i < secremain)   /* 需要擦除 */
        {
            W25Q128_Sector_Erase(secpos);  /* 擦除这个扇区 */

            for (i = 0; i < secremain; i++)   /* 复制 */
            {
                norflash_buf[i + secoff] = arr[i];
            }

            W25Q128_Write_NoCheck(secpos * 4096, 4096, norflash_buf);  /* 写入整个扇区 */
        }
        else        /* 写已经擦除了的,直接写入扇区剩余区间. */
        {
            W25Q128_Write_NoCheck(address, secremain, arr);  /* 直接写扇区 */
        }

        if (datalen == secremain)
        {
            break;  /* 写入结束了 */
        }
        else        /* 写入未结束 */
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */

            arr += secremain;      /* 指针偏移 */
            address += secremain;  /* 写地址偏移 */
            datalen -= secremain;  /* 字节数递减 */

            if (datalen > 4096)
            {
                secremain = 4096;   /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = datalen;/* 下一个扇区可以写完了 */
            }
        }
    }
}



/**********************************************************
* @funcName ：W25Q128_Sector_Erase
* @brief    ：W25Q128扇区擦除
* @param    ：uint32_t address  (w25Q128内部的存储地址，最大32bit)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Sector_Erase(uint32_t address)
{
    W25Q128_Write_Enable();          /* 写使能 */
    W25Q128_Wait_Busy();             /* 等待空闲 */
    /* 擦除 */
    SPI_CS_L;                        /* 片选使能 */
    SPI2_RS_Byte(FLASH_SectorErase); /* 扇区擦除指令 */
    W25Q128_Send_Address(address);   /* 发送地址 */
    SPI_CS_H;                        /* 片选失能 */
    W25Q128_Wait_Busy();             /* 等待擦除完成 */
}



/**********************************************************
* @funcName ：W25Q128_Block_Erase
* @brief    ：W25Q128块区擦除
* @param    ：uint32_t address  (w25Q128内部的存储地址，最大32bit)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Block_Erase(uint32_t address)
{
    W25Q128_Write_Enable();          /* 写使能 */
    W25Q128_Wait_Busy();             /* 等待空闲 */
    /* 擦除 */
    SPI_CS_L;                        /* 片选使能 */
    SPI2_RS_Byte(FLASH_BlockErase);  /* 块区擦除指令 */
    W25Q128_Send_Address(address);   /* 发送地址 */
    SPI_CS_H;                        /* 片选失能 */
    W25Q128_Wait_Busy();             /* 等待擦除完成 */
}



/**********************************************************
* @funcName ：W25Q128_Chip_Erase
* @brief    ：WW25Q128芯片擦除
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Chip_Erase(void)
{
    W25Q128_Write_Enable();          /* 写使能 */
    W25Q128_Wait_Busy();             /* 等待空闲 */
    /* 擦除 */
    SPI_CS_L;                        /* 片选使能 */
    SPI2_RS_Byte(FLASH_ChipErase);   /* 芯片擦除 */
    SPI_CS_H;                        /* 片选失能 */
    W25Q128_Wait_Busy();             /* 等待擦除完成 */
}



/**********************************************************
* @funcName ：W25Q128_Blocks_Erase
* @brief    ：W25Q128连续多个块擦除
* @param    ：uint32_t address   (w25Q128内部的存储地址，最大32bit)
* @param    ：uint8_t blockNum   (要擦除块的个数)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void W25Q128_Blocks_Erase(uint32_t address,uint8_t blockNum)
{
    while(blockNum)
    {
        W25Q128_Block_Erase(address);
        address += 65536;
        blockNum--;
    }
}












