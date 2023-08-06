#include "usart.h"



/**********************************************************
* @funcName ：USART1_Init
* @brief    ：对USART1对应的GPIO口进行初始化设置
* @param    ：uint32_t baud (波特率)
* @retval   ：void
* @details  ：
*             PA9     TX     ---------输出
*             PA10    RX     ---------输入
* @fn       ：
************************************************************/
void USART1_Init(uint32_t baud)
{
    /* GPIOx配置部分 */
    /* GPIOx时钟使能 */
    RCC->AHB1ENR |= (1<<0);

    /* 端口模式寄存器 */
    GPIOA->MODER &= ~((3<<2*9)|(3<<2*10));
    GPIOA->MODER |= ((2<<2*9)|(2<<2*10));

    /* 端口输出类型寄存器 */
    GPIOA->OTYPER &= ~(1<<9); 

    /* 端口输出速率寄存器 */
    GPIOA->OSPEEDR &= ~(3<<2*9);
    GPIOA->OSPEEDR |= (2<<2*9);

    /* 端口输入上拉/下拉寄存器 */
    GPIOA->PUPDR &= ~(3<<2*10);

    /* 端口复用功能寄存器 */
    GPIOA->AFR[1] &= ~((0xf<<4*(9-8))|(0xf<<4*(10-8)));
    GPIOA->AFR[1] |= (7<<4*(9-8)|(7<<4*(10-8)));

    /* 串口配置部分 */
    /* 串口时钟使能 */
    RCC->APB2ENR |= (1<<4);

    /* 控制寄存器CR1 */
    USART1->CR1 &= ~((1<<15)|(1<<12));
    USART1->CR1 |= ((1<<3)|(1<<2));

    /* 控制寄存器CR2 */
    USART1->CR2 &= ~(3<<12);

    //波特率寄存器
    USART1->BRR = 84*1000000/baud;

    /* 串口使能 */
    USART1->CR1 |= (1<<13);
}



/**********************************************************
* @funcName ：USART1_Send_Byte
* @brief    ：串口1发送一个字节的数据（uint8_t）
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Send_Byte(uint8_t data)
{
    while(!(USART1->SR & (1<<6)));
    USART1->DR = data;
}



/**********************************************************
* @funcName ：USART1_Receive_Byte
* @brief    ：串口1接收一个字节的数据（uint8_t）
* @param    ：void
* @retval   ：uint8_t
* @details  ：
* @fn       ：
************************************************************/
uint8_t USART1_Receive_Byte(void)
{
    uint8_t data;

    while(!(USART1->SR & (1<<5)));
    data = USART1->DR;

    return data;
}









