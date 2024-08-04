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
    /* GPIOx初始化设置 */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= (1<<2);

    /* 端口配置寄存器 */
    GPIOA->CRH &= ~((0xf<<4*(9-8))|(0xf<<4*(10-8)));
    GPIOA->CRH |= ((11<<4*(9-8))|(4<<4*(10-8)));

    /* 复用重映射寄存器 */
    AFIO->MAPR &= ~(1<<2);

    /* 外设复位寄存器 */
    RCC->APB2RSTR |= (1<<14);
    RCC->APB2RSTR &= ~(1<<14);

    /* 串口寄存器初始化设置 */
    /* 串口时钟使能 */
    RCC->APB2ENR |= (1<<14);

    /* 控制寄存器CR1 */
    USART1->CR1 &= ~(1<<12);
    USART1->CR1 |= ((1<<2)|(1<<3));

    /* 控制寄存器CR2 */
    USART1->CR2 &= ~(3<<12);

    /* 波特率寄存器 */
    USART1->BRR = 72*1000000 / baud;

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








