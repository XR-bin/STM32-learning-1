#ifndef __DMA_H
#define __DMA_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************    函数外部声明   *****************/
    void DMA2_Usart1_Init(uint8_t ch, uint8_t* data);   /* 对DMA2的通道4进行初始化设置(连接USART1的发送通道) */
    void DMA2_Enable(uint16_t ndtr);                    /* 开启DMA2数据传输 */

#endif



