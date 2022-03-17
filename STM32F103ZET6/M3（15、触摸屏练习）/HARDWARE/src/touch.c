#include "sys.h"
#include "touch.h"
#include "delay.h"

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
*            CS     PF11  通用输出
*            SCK    PB1   通用输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   通用输出
********************************************************/
void Touch_Init(void)
{
  //PB
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<3;
  //端口配置寄存器
  GPIOB->CRL &= ~((0xf<<(4*1))|(0xf<<(4*2)));
  GPIOB->CRL |= ((3<<(4*1))|(4<<(4*2)));
  
  //PF
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<7;
  //端口配置寄存器
  GPIOF->CRH &= ~((0xf<<(4*(9-8)))|(0xf<<(4*(10-8)))|(0xf<<(4*(11-8))));
  GPIOF->CRH |= ((3<<(4*(9-8)))|(4<<(4*(10-8)))|(3<<(4*(11-8))));
  
  //初始化状态空闲
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
        时钟线        T_SCK_H      T_SCK_L  
        输出数据线    T_MOSI_H     T_MOSI_L
        输入数据线    T_MISO 
        笔接触线      T_PEN    
********************************************************/
u16 Touch_Data(u8 command)
{
  u8 i;
  u16 data = 0;
  
  //数据线拉低
  T_MOSI_L;
  //片选线拉低
  T_CS_L;
  //循环8次发送指令数据
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
  //时钟线拉低
  T_SCK_L;
  //数据线拉低
  T_MOSI_L;
  //延时120us，用于等待ADC把数据转换好
  delay_us(120);
  //空出一个空脉冲
  T_SCK_L;
  delay_200ns();
  T_SCK_H;
  delay_200ns();
  //循环12次读取数据
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
    if(T_MISO) data |= 0x1;
    //延时200ns，用于读取数据
    delay_200ns();
  }
  //片选拉高
  T_CS_H;
  //时钟线拉低
  T_SCK_L;
  
  return data;
}






