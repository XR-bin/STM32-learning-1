#ifndef _W25Q128_H
#define _W25Q128_H

    /****************   Íâ²¿Í·ÎÄ¼þÉùÃ÷   ****************/
    #include "sys.h"



    /********************   ºê¶¨Òå   ********************/
    /* Æ¬Ñ¡ */
    #define SPI_CS_H  GPIOB->ODR |= (1<<14)    /* Æ¬Ñ¡Ê§ÄÜ */
    #define SPI_CS_L  GPIOB->ODR &= ~(1<<14)   /* Æ¬Ñ¡Ê¹ÄÜ */

    /* Ö¸Áî±í */
    #define FLASH_WriteEnable           0x06        /* Ð´Ê¹ÄÜ */
    #define FLASH_WriteDisable          0x04        /* Ð´Ê§ÄÜ */
    #define FLASH_ReadStatusReg1        0x05        /* ¶Á×´Ì¬¼Ä´æÆ÷1 */
    #define FLASH_ReadStatusReg2        0x35        /* ¶Á×´Ì¬¼Ä´æÆ÷2 */
    #define FLASH_ReadStatusReg3        0x15        /* ¶Á×´Ì¬¼Ä´æÆ÷3 */
    #define FLASH_WriteStatusReg1       0x01        /* Ð´×´Ì¬¼Ä´æÆ÷1 */
    #define FLASH_WriteStatusReg2       0x31        /* Ð´×´Ì¬¼Ä´æÆ÷2 */
    #define FLASH_WriteStatusReg3       0x11        /* Ð´×´Ì¬¼Ä´æÆ÷3 */
    #define FLASH_ReadData              0x03        /* ¶ÁÊý¾ÝÖ¸Áî */
    #define FLASH_FastReadData          0x0B 
    #define FLASH_FastReadDual          0x3B 
    #define FLASH_FastReadQuad          0xEB  
    #define FLASH_PageProgram           0x02        /* Ð´Ò³Ö¸Áî */
    #define FLASH_PageProgramQuad       0x32 
    #define FLASH_BlockErase            0xD8 
    #define FLASH_SectorErase           0x20        /* ÉÈÇø²Á³ý */
    #define FLASH_ChipErase             0xC7        /* Ð¾Æ¬²Á³ý */
    #define FLASH_PowerDown             0xB9 
    #define FLASH_ReleasePowerDown      0xAB 
    #define FLASH_DeviceID              0xAB 
    #define FLASH_ManufactDeviceID      0x90 
    #define FLASH_JedecDeviceID         0x9F 
    #define FLASH_Enable4ByteAddr       0xB7
    #define FLASH_Exit4ByteAddr         0xE9
    #define FLASH_SetReadParam          0xC0 
    #define FLASH_EnterQPIMode          0x38
    #define FLASH_ExitQPIMode           0xFF



    /****************    º¯ÊýÍâ²¿ÉùÃ÷   *****************/
    void W25Q128_Init(void);                             /* W25Q128³õÊ¼»¯ */
    uint8_t W25Q128_Read_SR(uint8_t serial);             /* ¶ÁÈ¡W25Q128×´Ì¬¼Ä´æÆ÷Öµ */
    void W25Q128_Write_SR(uint8_t serial, uint8_t data); /* ÐÞ¸ÄW25Q128×´Ì¬¼Ä´æÆ÷Öµ */
    static void W25Q128_Wait_Busy(void);                 /* µÈ´ýW25Q128¿ÕÏÐ */
    void W25Q128_Write_Enable(void);                     /* W25Q128Ð´Ê¹ÄÜ */
    static void W25Q128_Send_Address(uint32_t address);  /* ÏòW25QXX·¢ËÍµØÖ· */

    void W25Q128_Page_Write(uint32_t address, uint32_t datalen, uint8_t *arr);           /* Ð´Êý¾Ýµ½W25Q128(Ò³²Ù×Ý£¬²»¿É¿çÒ³) */
    static void W25Q128_Write_NoCheck(uint32_t address, uint16_t datalen, uint8_t *arr); /* Ð´Êý¾Ýµ½W25Q128(ÎÞ¼ìÑéÐ´²Ù×÷) */

    /* W25Q128¶ÁÐ´ÕýÊ½º¯Êý */
    void W25Q128_Read_Bytes(uint32_t address, uint32_t datalen, uint8_t *arr); /* ´ÓW25Q128¶ÁÈ¡Êý¾Ý */
    void W25Q128_Write(uint32_t address, uint16_t datalen, uint8_t *arr);      /* Ð´Êý¾Ýµ½W25Q128(´ø²Á²Ù×÷) */

    void W25Q128_Sector_Erase(uint32_t address);                    /* ÉÈÇø²Á³ý */
    void W25Q128_Block_Erase(uint32_t address);                     /* ¿éÇø²Á³ý */
    void W25Q128_Chip_Erase(void);                                  /* Ð¾Æ¬²Á³ý */
    void W25Q128_Blocks_Erase(uint32_t address,uint8_t blockNum);   /* ²Á³ý¶à¸ö¿é */

#endif




