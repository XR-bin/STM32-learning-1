#include "stm32f4xx.h"  
#include "stdio.h"

/******************************************************************
*函数功能  ：对SPI1通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
*函数名    ：SPI1_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*           PB3   SPI1_SCK   时钟线  推挽 
*           PB4   SPI1_MISO  主输入  无上下拉
*           PB5   SPI1_MOSI  主输出  推挽
*******************************************************************/
void SPI1_Init(void)
{
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<1);
  //端口模式寄存器
  GPIOB->MODER &= ~(0x3f<<6);
  GPIOB->MODER |= ((2<<(2*3))|(2<<(2*4))|(2<<(2*5)));
  //端口输出类型寄存器
  GPIOB->OTYPER &= ~(1<<3);
  GPIOB->OTYPER &= ~(1<<5);
  //端口输出速率寄存器
  GPIOB->OSPEEDR &= ~(3<<2*3);
  GPIOB->OSPEEDR &= ~(3<<2*5);
  GPIOB->OSPEEDR |= (2<<2*3);
  GPIOB->OSPEEDR |= (2<<2*5);
  //端口输入上拉/下拉寄存器
  GPIOB->PUPDR &= ~(3<<2*4);
  //端口复用功能寄存器
  GPIOB->AFR[0] &= (0xfff<<4*3);
  GPIOB->AFR[0] |= (0x555<<4*3);
  
  /*配置SPI寄存器*/
  //SPI时钟使能
  RCC->APB2ENR |= (1<<12);
  //控制寄存器CR1
  SPI1->CR1 &= ~(0xffff);
  SPI1->CR1 |= (1<<9);
  SPI1->CR1 |= (1<<8);
  SPI1->CR1 |= (1<<3);
  SPI1->CR1 |= (1<<2);
  //控制寄存器CR2
  SPI1->CR2 &= ~(0xff);
  //配置寄存器
  SPI1->I2SCFGR &= ~(0xfff);
  
  //使能SPI
  SPI1->CR1 |= 1<<6;
}

/******************************************************************
*函数功能  ：SPI1接收/发送一个字节数据（8位）
*函数名    ：SPI1_RS_Byte
*函数参数  ：u8 data
*函数返回值：u8
*描述      ：
*           PB3   SPI1_SCK   时钟线
*           PB4   SPI1_MISO  主输入
*           PB5   SPI1_MOSI  主输出
*******************************************************************/
u8 SPI1_RS_Byte(u8 data)
{
  //判断发送缓存区是否为空
  while(!(SPI1->SR & (1<<1)));
  SPI1->DR = data;
  //判断接收缓存区是否为空
  while(!(SPI1->SR & (1<<0)));
  data = SPI1->DR;
  
  return data;
}















