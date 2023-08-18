#ifndef __AT24CXX_H
#define __AT24CXX_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    #define AT24CXX_NO_ERR 0      /* 应答有效   数据传输成功 */
    #define AT24CXX_ERR1   1      /* 应答失效   器件地址数据传输失败 */
    #define AT24CXX_ERR2   2      /* 应答失效   字节地址数据传输失败 */
    #define AT24CXX_ERR3   3      /* 应答失效   要存储的数据传输失败 */
    #define AT24CXX_ERR4   4      /* 应答失效   出现跨页错误 */
    #define AT24CXX_WRITE  0xa0   /* 器件地址   写操作 */
    #define AT24CXX_READ   0xa1   /* 器件地址   读操作 */

    /* 器件型号 */
    #define AT24C01     127        /* 8个字节为一页 */
    #define AT24C02     255        /* 8个字节为一页 */
    #define AT24C04     511        /* 16个字节为一页 */
    #define AT24C08     1023       /* 16个字节为一页 */
    #define AT24C16     2047       /* 16个字节为一页 */
    #define AT24C32     4095       /* 32个字节为一页 */
    #define AT24C64     8191       /* 32个字节为一页 */
    #define AT24C128    16383      /* 64个字节为一页 */
    #define AT24C256    32767      /* 64个字节为一页 */

    /* 开发板使用的是24c02，所以定义AT24CXX为AT24C02 */
    #define    AT24CXX          AT24C02
    #define    AT24CXX_PAGE     8



    /****************    函数外部声明   *****************/
    void AT24CXX_Init(void);/* 初始化 */

    /* 单AT24CXX */
    uint8_t AT24CXX_Write_Byte(uint16_t address,uint8_t data);                       /* 单个字节写 */
    uint8_t AT24CXX_Read_Byte(uint16_t address,uint8_t *data);                       /* 单个字节读 */
    uint8_t AT24CXX_Write_Bytes1(uint16_t address,uint8_t byte_num,uint8_t *arr);    /* 多个字节写不可跨页 */
    uint8_t AT24CXX_Write_Bytes(uint16_t address,uint8_t byte_num,uint8_t *arr);     /* 多个字节写可跨页(手动跨页) */
    uint8_t AT24CXX_Read_Bytes(uint16_t address,uint8_t byte_num,uint8_t *arr);      /* 多个字节读可跨页(硬件自动跨页) */
    uint8_t AT24CXX_Write_Addr(uint16_t address,uint8_t byte_num,uint8_t *arr);      /* 多个字节写可跨页(一个地址一个地址写，无需考虑跨页) */
    uint8_t AT24CXX_Read_Addr(uint16_t address,uint8_t byte_num,uint8_t *arr);       /* 多个字节读可跨页(一个地址一个地址写，无需考虑跨页) */

    /* 多AT24CXX */
    uint8_t AT24CXXaddr_Write_Byte(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t data);      /* 单个字节写 */
    uint8_t AT24CXXaddr_Read_Byte(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t *data);      /* 单个字节读 */
    uint8_t AT24CXXaddr_Write_Bytes1(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* 多个字节写不可跨页 */
    uint8_t AT24CXXaddr_Write_Bytes(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint8_t AT24CXXpage,uint16_t address,uint8_t byte_num,uint8_t *arr);    /* 多个字节写可跨页(手动跨页) */
    uint8_t AT24CXXaddr_Read_Bytes(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* 多个字节读可跨页(硬件自动跨页) */
    uint8_t AT24CXXaddr_Write_Addr(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);     /* 多个字节写可跨页(一个地址一个地址写，无需考虑跨页) */
    uint8_t AT24CXXaddr_Read_Addr(uint16_t AT24CXXmodel,uint8_t AT24CXXaddr,uint16_t address,uint8_t byte_num,uint8_t *arr);      /* 多个字节读可跨页(一个地址一个地址写，无需考虑跨页) */

#endif






