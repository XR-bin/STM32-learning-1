#include "sys.h"

/*****************************************************
*函数功能  ：对DMA1的通道4进行初始化设置
*函数名    ：DMA1_Channels4_Init
*函数参数  ：void
*函数返回值：u16 ndtr,u8 *data 
*描述      ：
*           DMA将存储器的数据搬运到串口1外设
********************************************************/
void DMA1_Channels4_Init(u16 ndtr,u8 *data)
{
  //DMA1时钟使能
  RCC->AHBENR |= 1<<0;
  //DMA通道x配置寄存器
  DMA1_Channel4->CCR &= ~(1<<14);    //非存储器到存储器模式
  DMA1_Channel4->CCR &= ~(3<<12);    //通道优先级   低
  DMA1_Channel4->CCR &= ~(3<<10);    //存储器数据宽度  8位
  DMA1_Channel4->CCR &= ~(3<<8);     //外设数据宽度    8位
  DMA1_Channel4->CCR |= (1<<7);      //执行存储器地址增量操作
  DMA1_Channel4->CCR &= ~(1<<0);     //不执行外设地址增量模式 
  DMA1_Channel4->CCR |= (1<<4);      //数据传输方向 
  //DMA通道x传输数量寄存器
  DMA1_Channel4->CNDTR = 0;
  DMA1_Channel4->CNDTR = ndtr;            //设置传输数量
  //DMA通道x外设地址寄存器
  DMA1_Channel4->CPAR = 0;
  DMA1_Channel4->CPAR = (u32)&USART1->DR;  //设置外设地址
  //DMA通道x外设地址寄存器
  DMA1_Channel4->CMAR = 0;
  DMA1_Channel4->CMAR = (u32)data;         //设置存储地址
  //使能串口DMA发送
  USART1->CR3 |= 1<<7;            
  //使能DMA1
  DMA1_Channel4->CCR |= 1<<0;              //通道开启
}

















