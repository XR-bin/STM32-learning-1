#include "stm32f4xx.h"  
#include "touch.h"
#include "delay.h"
#include "stdio.h"

//200ns延时函数
static void delay_200ns(void)
{
  u8 i = 20;
  while(i) i--;
}

/*****************************************************
*函数功能  ：对触摸屏芯片对应的GPIO口进行初始化设置
*函数名    ：Touch_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            CS     PC13 通用输出
*            SCK    PB0  通用输出
*            PEN    PB1  通用输入
*            MISO   PB2  通用输入
*            MOSI   PF11 通用输出
********************************************************/
void Touch_Init(void)
{
  //PB
  //GPIOx时钟使能
  RCC->AHB1ENR |= 1<<1;
  //端口模式寄存器
  GPIOB->MODER &= ~((3<<(2*0))|(3<<(2*1))|(3<<(2*2)));
  GPIOB->MODER |= 1<<(2*0);
  //端口输出类型寄存器
  GPIOB->OTYPER &= ~(1<<0);
  //端口输出速度寄存器
  GPIOB->OSPEEDR &= ~(3<<(2*0));
  GPIOB->OSPEEDR |= (2<<(2*0));
  //端口上拉/下拉寄存器
  GPIOB->PUPDR &= ~((3<<(2*1))|(3<<(2*2)));
  
  //PC
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<2);
  //端口模式寄存器
  GPIOC->MODER &= ~(3<<(2*13));
  GPIOC->MODER |= (1<<(2*13));
  //端口输出类型寄存器
  GPIOC->OTYPER &= ~(1<<13);
  //端口输出速度寄存器
  GPIOC->OSPEEDR &= ~(3<<(2*13));
  GPIOC->OSPEEDR |= (2<<(2*13));
  
  //PF
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<5);
  //端口模式寄存器
  GPIOF->MODER &= ~(3<<(2*11));
  GPIOF->MODER |= (1<<(2*11));
  //端口输出类型寄存器
  GPIOF->OTYPER &= ~(1<<11);
  //端口输出速度寄存器
  GPIOF->OSPEEDR &= ~(3<<(2*11));
  GPIOF->OSPEEDR |= (2<<(2*11));
  
  //初始状态空闲
  T_CS_H;
  T_SCK_L;
  T_MOSI_L;
}

/*****************************************************
*函数功能  ：通过触摸屏芯片获取触摸的坐标数据
*函数名    ：Touch_Data
*函数参数  ：u16
*函数返回值：u8 command 
*描述      ：
*       片选线        T_CS_H       T_CS_L  
*       时钟线        T_SCK_H      T_SCK_L  
*       输出数据线    T_MOSI_H     T_MOSI_L
*       输入数据线    T_MISO 
*       笔接触线      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u16 data = 0;
  u8 i;
  
  //拉低输出数据线
  T_MOSI_L;
  //拉低片选
  T_CS_L;
  //循环8次发送指令
  for(i=0;i<8;i++)
  {
    //时钟线拉低
    T_SCK_L;
    //判断指令位电平
    if(command & 0x80)T_MOSI_H;
    else T_MOSI_L;
    //延时200ns，用于数据建立
    delay_200ns();
    //时钟线拉高
    T_SCK_H;
    //延时200ns，用于数据读取
    delay_200ns();
    //指令数据左移
    command <<= 1;
  }
  //拉低数据输出线
  T_MOSI_L;
  //拉低时钟线
  T_SCK_L;
  //延时120us，用于等待ADC转换数据
  delay_us(120);
  //空出一个空脉冲
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //循环12次接收数据
  for(i=0;i<12;i++)
  {
    //指令数据左移
    data <<= 1;
    //时钟线拉低
    T_SCK_L;
    //延时200ns，用于数据建立
    delay_200ns();
    //时钟线拉高
    T_SCK_H;
    //读取数据位电平
    if(T_MISO) 
    {
      data |= 0x1;
    }
    //延时200ns，用于数据读取
    delay_200ns();
  }
  //片选拉高
  T_CS_H;
  //时钟线拉低
  T_SCK_L;
  
  return data;
}




