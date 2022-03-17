#ifndef _W25Q128_H
#define _W25Q128_H

  #include "sys.h"
  
  #define SPI_CS_H  GPIOB->ODR |= (1<<12);
  #define SPI_CS_L  GPIOB->ODR &= ~(1<<12);
  
  void W25Q128_Init(void);
  void W25Q128_Write_Enable(void);
  u8 W25Q128_Read_Status(void);
  void W25Q128_Read_Bytes(u32 address,u32 num_byte,u8 *str);
  void W25Q128_Page_Write(u32 address,u32 num_byte,u8 *str);
  void W25Q128_Sector_Erase(u32 address);
  void W25Q128_Block_Erase(u32 address);
  void W25Q128_Chip_Erase(void);
  void W25Q128_Blocks_Erase(u32 address,u8 num_blcok);
  void W25Q128_Block_Erase(u32 address);
  void W25Q128_Chip_Erase(void);
  void W25Q128_Blocks_Erase(u32 address,u8 num_blcok);

#endif



