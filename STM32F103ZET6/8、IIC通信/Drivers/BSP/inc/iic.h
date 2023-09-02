#ifndef __IIC_H
#define __IIC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* SCL */
    #define SCL_H      (GPIOB->ODR |= (1<<6))    /* 时钟线拉高 */
    #define SCL_L      (GPIOB->ODR &= ~(1<<6))   /* 时钟线拉低 */

    /* SDA */
    /* 输出： */
    #define SDA_OUT_H  (GPIOB->ODR |= (1<<7))    /* 输出模式下数据线拉高 */
    #define SDA_OUT_L  (GPIOB->ODR &= ~(1<<7))   /* 输出模式下数据线拉低 */
    /* 输入： */
    #define SDA_IN     (GPIOB->ODR |= (1<<7))    /* 输入模式 */
    #define SDA_INT    (GPIOB->IDR & (1<<7))     /* 接收到的值 */



    /****************    函数外部声明   *****************/
    void IIC_Init(void);                /* IIC初始化 */
    void IIC_Start(void);               /* IIC起始信号 */
    void IIC_Stop(void);                /* IIC停止信号 */
    void IIC_Send_Ack(uint8_t ack);     /* IIC应答信号 */
    uint8_t IIC_Receive_Ack(void);      /* 接收IIC应答信号 */
    void IIC_Send_Byte(uint8_t data);   /* 发送一个字节数据 */
    uint8_t IIC_Receive_Byte(void);     /* 接收一个字节数据 */

#endif











