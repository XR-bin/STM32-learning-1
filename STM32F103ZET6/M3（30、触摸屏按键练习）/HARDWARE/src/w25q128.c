#include "sys.h"
#include "spi.h"
#include "w25q128.h"
#include "stdio.h"

/*********************************************
*W25Q128存储芯片
*容量大小             ：16Mbyte  或  128Mbit
*共有块区             ：256块
*每块区共有扇区       ：16扇区
*每扇区共有页         ：16页
*每页字节             ：256字节
*地址可写最大十六进制数：0x9fffff
*        9f:块区   f：扇区    f：页    ff：字节
**********************************************/


/*******************************************
*函数功能  ：W25Q128初始化
*函数名    ：W25Q128_Init
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*           SPI_CS     PB12
********************************************/
void W25Q128_Init(void)
{
  SPI2_Init();
  /*片选引脚*/
  //GPIOx时钟
  RCC->APB2ENR |= 1<<3;
  //端口配置寄存器
  GPIOB->CRH &= ~(0xf<<(4*(12-8)));
  GPIOB->CRH |= (0x3<<(4*(12-8)));
  //端口输出数据寄存器(片选拉高)
  GPIOB->ODR |= 1<<12;
}

/*******************************************
*函数功能  ：对W25Q128进行写使能
*函数名    ：W25Q128_Write_Enable
*函数参数  ：void
*函数返回值：void
*函数描述  ：写使能指令    0x06
*            此函数用于写、擦除
********************************************/
void W25Q128_Write_Enable(void)
{
  SPI_CS_L;
  SPI2_RS_Byte(0x06);
  SPI_CS_H;
}

/*******************************************
*函数功能  ：读W25Q128的状态寄存器的值
*函数名    ：W25Q128_Read_Status
*函数参数  ：void
*函数返回值：u8
*函数描述  ：读状态指令    0x05
*            此函数用于写、擦除
*            用返回值判断是否写入完成
********************************************/
u8 W25Q128_Read_Status(void)
{
  u8 status;
  
  SPI_CS_L;
  SPI2_RS_Byte(0x05);
  status = SPI2_RS_Byte(0xff);
  SPI_CS_H;
  
  return status;
}

/*****************************************************
*函数功能  ：主控芯片从W25Q128的某个地址读n个字节数据
*函数名    ：W25Q128_Read_Bytes
*函数参数  ：void
*函数返回值：u32 address  u32 num_byte  u8 *str
*函数描述  ：读指令    0x03
*******************************************************/
void W25Q128_Read_Bytes(u32 address,u32 num_byte,u8 *str)
{
  SPI_CS_L;
  //发指令
  SPI2_RS_Byte(0x03);
  //发地址
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  //收数据
  while(num_byte)
  {
    *str = SPI2_RS_Byte(0xff);
    str++;
    num_byte--;
  }
  SPI_CS_H;
}

/***************************************************************
*函数功能  ：主控芯片从W25Q128的某个地址连写n个字节数据(不可跨页)
*函数名    ：W25Q128_Page_Write
*函数参数  ：void
*函数返回值：u32 address  u32 num_byte  u8 *str
*函数描述  ：页写取指令    0x02
****************************************************************/
void W25Q128_Page_Write(u32 address,u32 num_byte,u8 *str)
{
  //写使能
  W25Q128_Write_Enable();
  //写数据过程
  SPI_CS_L;
  SPI2_RS_Byte(0x02);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  
  while(num_byte)
  {
    SPI2_RS_Byte(*str);
    str++;
    num_byte--;
  }
  SPI_CS_H;
  //等待写完过程
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q128扇区擦除
*函数名    ：W25Q64_Sector_Erase
*函数参数  ：void
*函数返回值：u32 address
*函数描述  ：扇区擦除指令    0x20
****************************************************************/
void W25Q128_Sector_Erase(u32 address)
{
  //写使能
  W25Q128_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI2_RS_Byte(0x20);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q128块区擦除
*函数名    ：W25Q128_Block_Erase
*函数参数  ：void
*函数返回值：u32 address
*函数描述  ：块擦除指令    0xd8
****************************************************************/
void W25Q128_Block_Erase(u32 address)
{
  //写使能
  W25Q128_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI2_RS_Byte(0xd8);
  SPI2_RS_Byte(address>>16);
  SPI2_RS_Byte(address>>8);
  SPI2_RS_Byte(address);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q128芯片擦除
*函数名    ：W25Q128_Chip_Erase
*函数参数  ：void
*函数返回值：void
*函数描述  ：芯片擦除指令    0xc7
****************************************************************/
void W25Q128_Chip_Erase(void)
{
  //写使能
  W25Q128_Write_Enable();
  //擦除
  SPI_CS_L;
  SPI2_RS_Byte(0xc7);
  SPI_CS_H;
  //等待擦除完成
  while(W25Q128_Read_Status() & (1<<0));
}

/***************************************************************
*函数功能  ：W25Q128连续多个块擦除
*函数名    ：W25Q128_Blocks_Erase
*函数参数  ：void
*函数返回值：void
*函数描述  ：
****************************************************************/
void W25Q128_Blocks_Erase(u32 address,u8 num_blcok)
{
  while(num_blcok)
	{
		W25Q128_Block_Erase(address);
		address += 65536;
		num_blcok--;
	}
}












