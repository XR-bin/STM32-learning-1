#include "stmflash.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\stmflash.c
*              .\Drivers\BSP\inc\stmflash.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-12-12
* @brief     ：STM32内部Flash读写操作
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：STMFLASH_Unlock
* @brief    ：解锁STM32的FLASH
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
static void STMFLASH_Unlock(void)
{
    FLASH->KEYR = STM32_FLASH_KEY1;     /* FLASH 写入解锁序列. */
    FLASH->KEYR = STM32_FLASH_KEY2;
}



/**********************************************************
* @funcName ：STMFLASH_Lock
* @brief    ：flash上锁
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
static void STMFLASH_Lock(void)
{
    FLASH->CR |= 1 << 7;    /* FLASH 上锁 */
}



/**********************************************************
* @funcName ：STMFLASH_Get_Status
* @brief    ：得到FLASH的状态
* @param    ：void
* @retval   ：uint8_t --- (flash当前状态，0-无错误、1-忙、2-错误、3-写保护错误)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t STMFLASH_Get_Status(void)
{
    uint32_t res;
    res = FLASH->SR;

    if(res & (1 << 0))return 1;    /* BSY = 1      , 忙 */
    if(res & (1 << 2))return 2;    /* PGERR = 1    , 编程错误*/
    if(res & (1 << 4))return 3;    /* WRPRTERR = 1 , 写保护错误 */
    
    return 0;   /* 没有任何错误 操作完成. */
}



/**********************************************************
* @funcName ：STMFLASH_Wait_Done
* @brief    ：等待操作完成
* @param    ：uint32_t time (等待时间上限)
* @retval   ：uint8_t --- (flash当前状态，0-无错误、1-忙、2-错误、3-写保护错误、0xff-超时)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t STMFLASH_Wait_Done(uint32_t time)
{
    uint8_t res;

    do
    {
        res = STMFLASH_Get_Status();

        if (res != 1)
        {
            break;      /* 非忙, 无需等待了, 直接退出 */
        }

        time--;
    }while (time);

    if(time == 0)res = 0XFF;   /* 超时 */

    return res;
}



/**********************************************************
* @funcName ：STMFLASH_Erase_Sector
* @brief    ：擦除扇区
* @param    ：vuint32_t saddr (扇区地址 0 ~ 256)
* @retval   ：uint8_t --- (flash当前状态，0-无错误、1-忙、2-错误、3-写保护错误、0xff-超时)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t STMFLASH_Erase_Sector(uint32_t saddr)
{
    uint8_t res = 0;
    res = STMFLASH_Wait_Done(0X5FFFFF);                       /* 等待上次操作结束, >20ms */

    if(res == 0)
    {
        FLASH->CR |= 1 << 1;    /* 页擦除 */
        FLASH->AR = saddr;      /* 设置页地址(实际是半字地址) */
        FLASH->CR |= 1 << 6;    /* 开始擦除 */
        res = STMFLASH_Wait_Done(0X5FFFFF); /* 等待操作结束, >20ms */

        if(res != 1)   /* 非忙 */
        {
            FLASH->CR &= ~(1 << 1); /* 清除页擦除标志 */
        }
    }

    return res;
}



/**********************************************************
* @funcName ：STMFLASH_Read_HalfWord
* @brief    ：读取指定地址的半字(16位数据)
* @param    ：uint32_t faddr(读地址,此地址必须为2的倍数!!)
* @retval   ：uint16_t --- (地址中的数据)
* @details  ：
* @fn       ：
************************************************************/
uint16_t STMFLASH_Read_HalfWord(uint32_t faddr)
{
    return *(volatile uint16_t*)faddr; 
}



/**********************************************************
* @funcName ：STMFLASH_Read
* @brief    ：从指定地址开始读出指定长度的数据
* @param    ：uint32_t rAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要读的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Read(uint32_t rAddr, uint16_t *pBuf, uint16_t length)
{
    uint16_t i;

    for(i=0; i<length; i++)
    {
        pBuf[i]=STMFLASH_Read_HalfWord(rAddr);/* 读取2个字节 */
        rAddr+=2;                             /* 偏移2个字节 */
    }
}



/**********************************************************
* @funcName ：STMFLASH_Write_HalfWord
* @brief    ：在FLASH指定地址写入半字 (16位数据)
* @param    ：uint32_t faddr(读地址,此地址必须为2的倍数!!)
* @param    ：uint16_t data(要写入的数据)
* @retval   ：uint8_t --- (flash当前状态，0-无错误、1-忙、2-错误、3-写保护错误、0xff-超时)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t STMFLASH_Write_HalfWord(uint32_t faddr, uint16_t data)
{
    uint8_t res;
    res = STMFLASH_Wait_Done(0X5FFFFF);

    if(res == 0)       /* OK */
    {
        FLASH->CR |= 1 << 0;                /* 编程使能 */
        *(volatile uint16_t *)faddr = data; /* 写入数据 */
        res = STMFLASH_Wait_Done(0X5FFFFF); /* 等待操作完成 */

        if(res != 1)   /* 操作成功 */
        {
            FLASH->CR &= ~(1 << 0);         /* 清除PG位 */
        }
    }

    return res;
}


/**********************************************************
* @funcName ：STMFLASH_Write_NoCheck
* @brief    ：从指定地址开始写入指定长度的数据(不检查的写入)
* @param    ：uint32_t wAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要写的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Write_NoCheck(uint32_t wAddr, uint16_t *pBuf, uint16_t length)
{
    uint16_t i;

    for(i=0; i<length; i++)
    {
        STMFLASH_Write_HalfWord(wAddr, pBuf[i]);  /* 写数据 */
        wAddr+=2;                                 /* 地址增加2 */
    }  
}



uint16_t STMFLASH_BUF[STM32_SECTOR_SIZE/2];     /* 读取数据的缓存，最多是2K字节 */

/**********************************************************
* @funcName ：STMFLASH_Write
* @brief    ：从指定地址开始写入指定长度的数据(有检查的写入)
* @param    ：uint32_t wAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要写的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Write(uint32_t wAddr, uint16_t *pBuf, uint16_t length)
{
    uint32_t secpos;        /* 扇区地址 */
    uint16_t secoff;        /* 扇区内偏移地址(16位字计算) */
    uint16_t secremain;     /* 扇区内剩余地址(16位字计算) */
    uint16_t i;
    uint32_t offaddr;       /* 去掉0X08000000后的地址 */

    if (wAddr < STM32_FLASH_BASE || (wAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
    {
        return;      /* 非法地址 */
    }

    STMFLASH_Unlock();  /* 解锁 */

    offaddr = wAddr - STM32_FLASH_BASE;         /* 实际偏移地址 */
    secpos = offaddr / STM32_SECTOR_SIZE;       /* 扇区地址  0~127 for STM32F103RBT6 */
    secoff = (offaddr % STM32_SECTOR_SIZE) / 2; /* 在扇区内的偏移(2个字节为基本单位.) */
    secremain = STM32_SECTOR_SIZE / 2 - secoff; /* 扇区剩余空间大小 */

    if (length <= secremain)
    {
        secremain = length; /* 不大于该扇区范围 */
    }

    while(1)
    {
        /* 读出整个扇区的内容 */
        STMFLASH_Read(secpos*STM32_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM32_SECTOR_SIZE/2);
        for(i = 0; i < secremain; i++)     /* 校验数据 */
        {
            if(STMFLASH_BUF[secoff + i] != 0XFFFF)
            {
                break;      /* 需要擦除 */
            }
        }

        if(i < secremain)  /* 需要擦除 */
        {
            /* 擦除这个扇区 */
            STMFLASH_Erase_Sector(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE);

            for(i=0;i<secremain;i++)    /* 复制 */
            {
                STMFLASH_BUF[i+secoff] = pBuf[i];
            }

            /* 写入整个扇区 */
            STMFLASH_Write_NoCheck(secpos*STM32_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM32_SECTOR_SIZE/2);
        }
        else
        {
            /* 写已经擦除了的,直接写入扇区剩余区间 */
            STMFLASH_Write_NoCheck(wAddr, pBuf, secremain);
        }

        if(length == secremain)
        {
            break;//写入结束了
        }
        else
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */
            pBuf += secremain;      /* 指针偏移 */
            wAddr += secremain * 2; /* 写地址偏移(16位数据地址,需要*2) */
            length -= secremain;    /* 字节(16位)数递减 */

            if (length > (STM32_SECTOR_SIZE / 2))
            {
                secremain = STM32_SECTOR_SIZE / 2; /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = length;                /* 下一个扇区可以写完了 */
            }
        }
    }

    STMFLASH_Lock();       /* 上锁 */
}

