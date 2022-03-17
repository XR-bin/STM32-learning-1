#include "stm32f4xx.h"      

/*****************************************************
*函数功能  ：对DMA2的数据流7通道4进行初始化设置
*函数名    ：DMA2_Stream7ch4_Init
*函数参数  ：void
*函数返回值：u16 ndtr,u16 psc  
*描述      ：
*           DMA将存储器的数据搬运到串口1外设
********************************************************/
void DMA2_Stream7ch4_Init(u16 ndtr,u8 *data)
{
  //DMA2的时钟使能
  RCC->AHB1ENR |= (1<<22);
  //关闭数据流使能
  DMA2_Stream7->CR &= ~(1<<0);
  //数据流x配置寄存器
  DMA2_Stream7->CR &= ~(7<<25);
  DMA2_Stream7->CR |= (4<<25);   //通道选择
  DMA2_Stream7->CR &= ~(3<<23);  //存储器突发单次传输
  DMA2_Stream7->CR &= ~(3<<21);  //外设突发单次传输
  DMA2_Stream7->CR &= ~(3<<16);  //低优先级
  DMA2_Stream7->CR &= ~(3<<13);  //存储器数据大小8字节（每次从存储器运输8字节）
  DMA2_Stream7->CR &= ~(3<<11);  //外设数据大小8字节(每次从外设搬运8字节)
  DMA2_Stream7->CR |= (1<<10);   //每次数据传输后，存储器地址指针递增
  DMA2_Stream7->CR &= ~(1<<9);   //外设地址指针固定
  DMA2_Stream7->CR |= (1<<6);    //数据从存储器向外设方向传输
  DMA2_Stream7->CR &= ~(1<<5);   //DMA控制流器
  //数据流x数据项数据寄存器
  DMA2_Stream7->NDTR = ndtr;     //传输数据个数
  //数据流x外设地址寄存器
  DMA2_Stream7->PAR = (u32)&USART1->DR;
  //数据流x存储器0地址寄存器
  DMA2_Stream7->M0AR = (u32)data;
  
  //串口DMA使能
  USART1->CR3 |= (1<<7);
  
  //数据流使能
  DMA2_Stream7->CR |= (1<<0);
}














