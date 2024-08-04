#ifndef __NORFLASH_H
#define __NORFLASH_H

    /****************   Íâ²¿Í·ÎÄ¼þÉùÃ÷   ****************/
    #include "sys.h"
    #include "usart.h"



    /********************   ºê¶¨Òå   ********************/
    /* Æ¬Ñ¡ */
    #define SPI_CS_H  GPIOB->ODR |= (1<<12)    /* Æ¬Ñ¡Ê§ÄÜ */
    #define SPI_CS_L  GPIOB->ODR &= ~(1<<12)   /* Æ¬Ñ¡Ê¹ÄÜ */

    /* FLASHÐ¾Æ¬ÁÐ±í */
    #define W25Q80      0XEF13          /* W25Q80   Ð¾Æ¬ID */
    #define W25Q16      0XEF14          /* W25Q16   Ð¾Æ¬ID */
    #define W25Q32      0XEF15          /* W25Q32   Ð¾Æ¬ID */
    #define W25Q64      0XEF16          /* W25Q64   Ð¾Æ¬ID */
    #define W25Q128     0XEF17          /* W25Q128  Ð¾Æ¬ID */
    #define W25Q256     0XEF18          /* W25Q256  Ð¾Æ¬ID */
    #define BY25Q64     0X6816          /* BY25Q64  Ð¾Æ¬ID */
    #define BY25Q128    0X6817          /* BY25Q128 Ð¾Æ¬ID */
    #define NM25Q64     0X5216          /* NM25Q64  Ð¾Æ¬ID */
    #define NM25Q128    0X5217          /* NM25Q128 Ð¾Æ¬ID */

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
    #define FLASH_ManufactDeviceID      0x90        /* »ñÈ¡Ð¾Æ¬ID */
    #define FLASH_JedecDeviceID         0x9F 
    #define FLASH_Enable4ByteAddr       0xB7
    #define FLASH_Exit4ByteAddr         0xE9
    #define FLASH_SetReadParam          0xC0 
    #define FLASH_EnterQPIMode          0x38
    #define FLASH_ExitQPIMode           0xFF



    /****************    º¯ÊýÍâ²¿ÉùÃ÷   *****************/
    void NorFlash_Init(void);                 /* NorFlashÐ¾Æ¬Ïà¹ØÒý½Å³õÊ¼»¯ */
    uint16_t NorFlash_Read_ID(void);          /* ¶ÁÈ¡Ð¾Æ¬ID */

#endif



