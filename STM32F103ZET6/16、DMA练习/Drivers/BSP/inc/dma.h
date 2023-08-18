#ifndef __DMA_H
#define __DMA_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************    函数外部声明   *****************/
    void DMA1_Usart1_Init(uint8_t* data);   /* 对DMA1的通道4进行初始化设置(连接USART1的发送通道) */
    void DMA1_Enable(uint16_t cndtr);       /* 开启DMA1数据传输 */

#endif




