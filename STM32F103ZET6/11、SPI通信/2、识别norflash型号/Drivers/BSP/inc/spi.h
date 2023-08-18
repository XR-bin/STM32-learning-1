#ifndef __SPI_H
#define __SPI_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void SPI2_Init(void);                 /* SPI2配置初始化 */
    uint8_t SPI2_RS_Byte(uint8_t data);   /* SPI2收发函数 */

#endif






