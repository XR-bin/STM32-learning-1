#include "at24cxx.h"
#include "iic.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\at24cxx.c
*              .\Drivers\BSP\inc\at24cxx.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：at24cxx读写操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：AT24CXX_Init
* @brief    ：对at24cxx所需要的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            时钟线  IIC_SCL  PB6   推挽输出
*            数据线  IIC_SDA  PB7   开漏输出
*                 高电平处于空闲状态
* @fn       ：
************************************************************/
void AT24CXX_Init(void)
{
    IIC_Init();
}



/**********************************************************
* @funcName ：AT24CXX_Write_Byte
* @brief    ：写一个字节的数据到AT24CXX的某个内部地址里
* @param    ：uint16_t address (存储地址)
* @param    ：uint8_t  data    (数据)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXX_Write_Byte(uint16_t address,uint8_t data)
{
    uint8_t ack;

    IIC_Start();                      /* 主机发送起始信号 */

    /* 判断AT24CXX型号 */
    if(AT24CXX > AT24C16)             /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE); /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);    /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));   /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                           /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);   /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Send_Byte(data);          /* 主机发送数据 */
    ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Stop();          /* 主机发送停止信号 */
    delay_ms(6);         /* 延时6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ：AT24CXX_Read_Byte
* @brief    ：从AT24CXX的里读取1个字节的数据
* @param    ：uint16_t address (存储地址)
* @param    ：uint8_t  *data   (数据)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXX_Read_Byte(uint16_t address,uint8_t *data)
{
    uint8_t ack;

    IIC_Start();                    /* 主机发送起始信号 */

    /*判断AT24CXX型号*/
    if(AT24CXX > AT24C16)             /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE); /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);      /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();        /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                              /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);            /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();               /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXX_READ);    /* 主机发送器件地址(读) */
    ack = IIC_Receive_Ack();        /* 主机接收应答 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    *data = IIC_Receive_Byte();       /* 主机开始接收数据 */
    IIC_Send_Ack(1);                  /* 主机发送不应答信号 */
    IIC_Stop();                       /* 主机发送停止信号 */

    return ack;
}



/**********************************************************
* @funcName ：AT24CXX_Write_Bytes1
* @brief    ：写多个字节的数据到AT24CXX里（不能跨页）
* @param    ：uint16_t address  (存储地址)
* @param    ：uint8_t  byte_num (数据组长度)
* @param    ：uint8_t  *arr     (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXX_Write_Bytes1(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    IIC_Start();                   /* 主机发送起始信号 */

    /*判断AT24CXX型号*/
    if(AT24CXX > AT24C16)          /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);  /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();       /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();          /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                            /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                              /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);     /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();        /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    for(i=0;i<byte_num;i++)
    {
        IIC_Send_Byte(*arr);          /* 主机发送数据 */
        ack = IIC_Receive_Ack();      /* 主机接收应答 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            delay_ms(6);
            return AT24CXX_ERR3;
        }
        arr++;
    }

    IIC_Stop();        /* 主机发送停止信号 */
    delay_ms(6);       /* 延时6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ：AT24CXX_Write_Bytes
* @brief    ：写多个字节的数据到AT24CXX里（可跨页）
* @param    ：uint16_t address  (存储地址)
* @param    ：uint8_t  byte_num (数据组长度)
* @param    ：uint8_t  *arr     (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXX_Write_Bytes(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t sck = 0;
    uint8_t remain;

    while(1)
    {
        /* 计算当前要存储的页还剩多少字节可以写 */
        remain =AT24CXX_PAGE - address%AT24CXX_PAGE;
        if(remain >= byte_num)
        {
            sck = AT24CXX_Write_Bytes1(address, byte_num, arr);
            break;
        }
        else
        {
            sck = AT24CXX_Write_Bytes1(address, remain, arr);
            byte_num -= remain;   /* 减去写入的数量 */
            address += remain;    /* 计算存储新地址 */
            arr += remain;        /* 计算数组的新地址 */
        } 
    }
    return sck;
}



/**********************************************************
* @funcName ：AT24CXX_Read_Bytes
* @brief    ：从AT24CXX的里读取多个字节的数据
* @param    ：uint16_t address  (存储地址)
* @param    ：uint8_t  byte_num (数据组长度)
* @param    ：uint8_t  *arr     (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXX_Read_Bytes(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t ack;

    IIC_Start();                       /* 主机发送起始信号 */

    /*判断AT24CXX型号*/
    if(AT24CXX > AT24C16)              /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE);      /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();           /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();          /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                            /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXX_WRITE+((address/256)<<1));      /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                              /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);        /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();           /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXX_READ);    /* 主机发送器件地址(读) */
    ack = IIC_Receive_Ack();        /* 主机接收应答 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    while(1)
    {
        *arr = IIC_Receive_Byte();         /* 主机开始接收数据 */
        byte_num--;
        if(byte_num == 0)
        {
            IIC_Send_Ack(1);               /* 结束应答 */
            break;
        }
        IIC_Send_Ack(0);
        arr++;
    }

    IIC_Stop();              /* 主机发送停止信号 */

    return ack;
}



/**********************************************************
* @funcName ：AT24CXX_Write_Addr
* @brief    ：写多个字节的数据到AT24CXX里(直接一个一个地址写，不用考虑跨不跨页)
* @param    ：uint16_t address  (存储地址)
* @param    ：uint8_t  byte_num (数据组长度)
* @param    ：uint8_t  *arr     (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXX_Write_Addr(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXX_Write_Byte(address, *arr);
        address++;
        arr++;
    }

    return ack;
}



/**********************************************************
* @funcName ：AT24CXX_Read_Addr
* @brief    ：从AT24CXX的里读取多个字节的数据(直接一个一个地址写，不用考虑跨不跨页)
* @param    ：uint16_t address  (存储地址)
* @param    ：uint8_t  byte_num (数据组长度)
* @param    ：uint8_t  *arr     (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXX_Read_Addr(uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXX_Read_Byte(address, arr);
        address++;
        arr++;
    }

    return ack;
}




/**********************************************************
* @funcName ：AT24CXXaddr_Write_Byte
* @brief    ：(多个AT24CXX连接时)写一个字节的数据到某个AT24CXX的某个内部地址里
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  data         (数据)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXXaddr_Write_Byte(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr,uint16_t address, uint8_t data)
{
    uint8_t ack;

    IIC_Start();                    /* 主机发送起始信号 */

    /* 判断AT24CXX型号 */
    if(AT24CXXmodel > AT24C16)      /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);   /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();          /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                             /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));    /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                          /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);       /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();          /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Send_Byte(data);          /* 主机发送数据 */
    ack = IIC_Receive_Ack();      /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Stop();      /* 主机发送停止信号 */
    delay_ms(6);     /* 延时6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Read_Byte
* @brief    ：(多个AT24CXX连接时)从某个AT24CXX的某个存储地址里读取1个字节的数据
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  data         (数据)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXXaddr_Read_Byte(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t *data)
{
    uint8_t ack;

    IIC_Start();                        /* 主机发送起始信号 */

    /* 判断AT24CXX型号 */
    if(AT24CXXmodel > AT24C16)          /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);     /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();        /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);      /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();        /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                                /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));      /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                            /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);      /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();         /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                         /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXXaddr|0x1);      /* 主机发送器件地址(读) */
    ack = IIC_Receive_Ack();             /* 主机接收应答 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    *data = IIC_Receive_Byte();         /* 主机开始接收数据 */
    IIC_Send_Ack(1);                    /* 主机发送不应答信号 */
    IIC_Stop();                         /* 主机发送停止信号 */

    return ack;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Write_Bytes1
* @brief    ：(多个AT24CXX)从某个AT24CXX的某个存储地址开始写多个字节的数据到（不能跨页）
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  byte_num     (数据组长度)
* @param    ：uint8_t  *arr         (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXXaddr_Write_Bytes1(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    IIC_Start();                       /* 主机发送起始信号 */

    /* 判断AT24CXX型号 */
    if(AT24CXXmodel > AT24C16)         /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr);        /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();           /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);        /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();          /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                              /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));     /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                           /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);      /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();         /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    for(i=0;i<byte_num;i++)
    {
        IIC_Send_Byte(*arr);       /* 主机发送数据 */
        ack = IIC_Receive_Ack();   /* 主机接收应答 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            delay_ms(6);
            return AT24CXX_ERR3;
        }
        arr++;
    }

    IIC_Stop();       /* 主机发送停止信号 */
    delay_ms(6);      /* 延时6ms */

    return AT24CXX_NO_ERR;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Write_Bytes
* @brief    ：(多个AT24CXX)从某个AT24CXX的某个存储地址开始写多个字节的数据到（可跨页）
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  byte_num     (数据组长度)
* @param    ：uint8_t  *arr         (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXXaddr_Write_Bytes(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint8_t AT24CXXpage, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t sck = 0;
    uint8_t remain;

    while(1)
    {
        /* 计算当前要存储的页还剩多少字节可以写 */
        remain =AT24CXXpage - address%AT24CXXpage;
        if(remain >= byte_num)
        {
            sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,byte_num,arr);
            break;
        }
        else
        {
            sck = AT24CXXaddr_Write_Bytes1(AT24CXXmodel,AT24CXXaddr,address,remain,arr);
            byte_num -= remain;   /* 减去写入的数量 */
            address += remain;    /* 计算存储新地址 */
            arr += remain;        /* 计算数组的新地址 */
        } 
    }
    return sck;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Read_Bytes
* @brief    ：(多个AT24CXX)从某个AT24CXX的某个地址开始读取多个字节的数据
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  byte_num     (数据组长度)
* @param    ：uint8_t  *arr         (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
*           AT24CXX_NO_ERR       0      应答有效   数据传输成功
*           AT24CXX_ERR1         1      应答失效   器件地址数据传输失败
*           AT24CXX_ERR2         2      应答失效   字节地址数据传输失败
*           AT24CXX_ERR3         3      应答失效   要存储的数据传输失败
*
*           AT24CXX_WRITE        0xa0   器件地址   写操作
*           AT24CXX_READ         0xa1   器件地址   读操作
************************************************************/
uint8_t AT24CXXaddr_Read_Bytes(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t ack;

    IIC_Start();                  /* 主机发送起始信号 */

    /* 判断AT24CXX型号 */
    if(AT24CXXmodel > AT24C16)    /* 如果是AT24C16以上的型号(不包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr); /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();    /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }

        IIC_Send_Byte(address>>8);   /* 发送高八位存储地址(字地址) */
        ack = IIC_Receive_Ack();     /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR2;
        }
    }
    else                           /* 如果是AT24C16以下的型号(包括AT24C16) */
    {
        IIC_Send_Byte(AT24CXXaddr+((address/256)<<1));     /* 主机发送器件地址(写) */
        ack = IIC_Receive_Ack();                           /* 接收从机应答信号 */

        /* 判断应答 */
        if(ack != 0)
        {
            IIC_Stop();
            return AT24CXX_ERR1;
        }
    }

    IIC_Send_Byte(address%256);        /* 发送低八位存储地址(字地址) */
    ack = IIC_Receive_Ack();           /* 接收从机应答信号 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    IIC_Start();                    /* 主机发送起始信号 */
    IIC_Send_Byte(AT24CXXaddr|1);   /* 主机发送器件地址(读) */
    ack = IIC_Receive_Ack();        /* 主机接收应答 */

    /* 判断应答 */
    if(ack != 0)
    {
        IIC_Stop();
        return AT24CXX_ERR2;
    }

    while(1)
    {
        *arr = IIC_Receive_Byte();        /* 主机开始接收数据 */
        byte_num--;

        if(byte_num == 0)
        {
            IIC_Send_Ack(1);        /* 结束应答 */
            break;
        }

        IIC_Send_Ack(0);
        arr++;
    }

    IIC_Stop();   /* 主机发送停止信号 */

    return ack;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Write_Addr
* @brief    ：(多个AT24CXX)写多个字节的数据到AT24CXX里(直接一个一个地址写，不用考虑跨不跨页)
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  byte_num     (数据组长度)
* @param    ：uint8_t  *arr         (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXXaddr_Write_Addr(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;
    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXXaddr_Write_Byte(AT24CXXmodel,AT24CXXaddr,address,*arr);
        address++;
        arr++;
    }
    return ack;
}



/**********************************************************
* @funcName ：AT24CXXaddr_Read_Addr
* @brief    ：从AT24CXX的里读取多个字节的数据(直接一个一个地址写，不用考虑跨不跨页)
* @param    ：uint16_t AT24CXXmodel (器件型号)
* @param    ：uint8_t  AT24CXXaddr  (器件地址(写))
* @param    ：uint16_t address      (存储地址)
* @param    ：uint8_t  byte_num     (数据组长度)
* @param    ：uint8_t  *arr         (数据组)
* @retval   ：uint8_t  ---  (操作情况)
* @details  ：
* @fn       ：
************************************************************/
uint8_t AT24CXXaddr_Read_Addr(uint16_t AT24CXXmodel, uint8_t AT24CXXaddr, uint16_t address, uint8_t byte_num, uint8_t *arr)
{
    uint8_t i;
    uint8_t ack;

    for(i=0;i<byte_num;i++)
    {
        ack = AT24CXXaddr_Read_Byte(AT24CXXmodel,AT24CXXaddr,address,arr);
        address++;
        arr++;
    }
    return ack;
}







