#ifndef _SPI_H
#define _SPI_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void SPI1_Init(void);                 /* SPI1配置初始化 */
    uint8_t SPI1_RS_Byte(uint8_t data);   /* SPI1收发函数 */

#endif



