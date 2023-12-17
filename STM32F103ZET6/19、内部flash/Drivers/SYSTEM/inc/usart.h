#ifndef __USART_H
#define __USART_H

    /****************   外部头文件声明   ****************/
    #include "sys.h" 
    #include "stdio.h"



    /****************    函数外部声明   *****************/
    /*串口1*/
    void USART1_Init(uint32_t baud);          /* 串口1初始化 */
    void USART1_Send_Byte(uint8_t data);      /* 串口1发生一个字节 */
    uint8_t USART1_Receive_Byte(void);        /* 串口1接收一个字节 */
    void USART1_Send_Str(uint8_t *str);       /* 串口1发生一个字符串 */
    void USART1_Receive_Str(uint8_t *str);    /* 串口1接收一个字符串 */
    /*串口2*/
    void USART2_Init(uint32_t baud);          /* 串口2初始化 */
    void USART2_Send_Byte(uint8_t data);      /* 串口2发生一个字节 */
    uint8_t USART2_Receive_Byte(void);        /* 串口2接收一个字节 */

#endif






