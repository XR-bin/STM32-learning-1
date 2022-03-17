#include "stm32f4xx.h"  
#include "touch.h"
#include "delay.h"
#include "stdio.h"
#include "lcd.h"
#include "stdlib.h"

ADJUST LCD_Touch;//用于存放LCD与触摸屏的之间的比例系数和偏移量(也就是校准系数)

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
*函数参数  ：u8 command
*函数返回值：u16 
*描述      ：
*       片选线        T_CS_H       T_CS_L  
        时钟线        T_SCK_H      T_SCK_L  
        输出数据线    T_MOSI_H     T_MOSI_L
        输入数据线    T_MISO 
        笔接触线      T_PEN    
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

/*****************************************************
*函数功能  ：触摸屏数据滤波函数
*函数名    ：Touch_Smoothing
*函数参数  ：void
*函数返回值：TOUCH_XY
*描述      ：
********************************************************/
TOUCH_XY Touch_Smoothing(void)
{
  u8 i,j;
  u16 touch_x[5];
  u16 touch_y[5];
  u16 temp;
  u16 sum_x = 0,sum_y = 0;
  TOUCH_XY mean;
  
  //获取十次坐标数据
  for(i=0;i<5;i++)
  {
    touch_x[i] = Touch_Data(TOUCH_X);
    touch_y[i] = Touch_Data(TOUCH_Y);
  }
  
  //对数据进行排序
  for(i=1;i<5;i++)
  {
    for(j=0;j<5-i;j++)
    {
      if(touch_x[j]>touch_x[j+1])
      {
        temp = touch_x[j];
        touch_x[j] = touch_x[j+1];
        touch_x[j+1] = temp;
      }
      if(touch_y[j]>touch_y[j+1])
      {
        temp = touch_y[j];
        touch_y[j] = touch_y[j+1];
        touch_y[j+1] = temp;
      }
    }
  }
  
  //去掉最大和最小，取平均值
  for(i=1;i<4;i++)
  {
    sum_x += touch_x[i];
    sum_y += touch_y[i];
  }
  mean.x = sum_x / 3;
  mean.y = sum_y / 3;
  
  return mean;
}

/*****************************************************
*函数功能  ：用于校准的十字提示函数
*函数名    ：LCD_Calibration_Cross
*函数参数  ：u16 x  u16 y  u16 colour
*函数返回值：void
*描述      ：
********************************************************/
void LCD_Calibration_Cross(u16 x,u16 y,u16 colour)
{
  u16 i;
  for(i=x-10;i<x+10;i++)
  {
    LCD_Point(i,y,colour);
  }
  for(i=y-10;i<y+10;i++)
  {
    LCD_Point(x,i,colour);
  }
}

/*****************************************************
*函数功能  ：LCD与触摸屏校准比例系数和偏移量
*函数名    ：LCD_Touch_Calibration
*函数参数  ：void
*函数返回值：void
*描述      ：
********************************************************/
void LCD_Touch_Calibration(void)
{
  u8 i;
  TOUCH_XY touch_xy[4];//用于存放LCD校准区域取的的触摸屏坐标
  TOUCH_XY lcd_xy[4] = {{20,20},{300,20},{20,460},{300,460}};//用于校准的LCD区域点坐标
  double temp1,temp2;

THIS:  
  /***************获取校准区域坐标***************/
  //通过特定的LCD坐标区域获取该区域的触摸屏坐标
  //LCD第0号校准区域坐标点(20,20)
  //LCD第1号校准区域坐标点(300,20)
  //LCD第2号校准区域坐标点(20,460)
  //LCD第3号校准区域坐标点(300,460)
  for(i=0;i<4;i++)
  {
    //画第i个区域十字
    LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,RED);
    //等待触摸屏按下
    while(T_PEN);
    //延时10ms消除按下抖动
    delay_ms(10);
    //获取触摸屏坐标
    touch_xy[i] = Touch_Smoothing(); 
    //等待抬起
    while(!T_PEN);
    //擦除区域十字
    LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,WHITE);
    //延时100ms，消除抬起抖动
    delay_ms(100);
  }

  /***************判断点的有效性***************/
  //求点0到点1的距离平方temp1
  temp1 = (touch_xy[0].x - touch_xy[1].x)*(touch_xy[0].x - touch_xy[1].x) + (touch_xy[0].y - touch_xy[1].y)*(touch_xy[0].y - touch_xy[1].y);
  //求点2到点3的距离平方temp2
  temp2 = (touch_xy[2].x - touch_xy[3].x)*(touch_xy[2].x - touch_xy[3].x) + (touch_xy[2].y - touch_xy[3].y)*(touch_xy[2].y - touch_xy[3].y);
  //判断是否在误差范围内
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //重新取点坐标
    goto THIS;
  }
  
  //求点0到点2的距离平方temp1
  temp1 = (touch_xy[0].x - touch_xy[2].x)*(touch_xy[0].x - touch_xy[2].x) + (touch_xy[0].y - touch_xy[2].y)*(touch_xy[0].y - touch_xy[2].y);
  //求点1到点3的距离平方temp2
  temp2 = (touch_xy[1].x - touch_xy[3].x)*(touch_xy[1].x - touch_xy[3].x) + (touch_xy[1].y - touch_xy[3].y)*(touch_xy[1].y - touch_xy[3].y);
  //判断是否在误差范围内
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //重新取点坐标
    goto THIS;
  }
  
  //求点0到点3的距离平方temp1
  temp1 = (touch_xy[0].x - touch_xy[3].x)*(touch_xy[0].x - touch_xy[3].x) + (touch_xy[0].y - touch_xy[3].y)*(touch_xy[0].y - touch_xy[3].y);
  //求点1到点2的距离平方temp2
  temp2 = (touch_xy[1].x - touch_xy[2].x)*(touch_xy[1].x - touch_xy[2].x) + (touch_xy[1].y - touch_xy[2].y)*(touch_xy[1].y - touch_xy[2].y);
  //判断是否在误差范围内
  if(((temp1/temp2)<0.97) || ((temp1/temp2)>1.03))
  {
    //重新取点坐标
    goto THIS;
  }
  
  /**********开始计算LCD和触摸屏的关系量并接收**********/
  //lcd和触摸屏的xy坐标的比例系数
  //取两点区域坐标(LCD与触摸屏)求比例系数(取两次，求平均值，减小误差)
  //注意:只针对对角线的点
  //公式: a_x = (LCD_x1 - LED_x2) / (触x1 - 触x2)
  //      c_y = (LCD_y1 - LED_y2) / (触y1 - 触y2)
  //备注: a_x、c_y：比例系数
  //      LCD_x1、LCD_y1：LCD第一个坐标点的坐标
  //      LCD_x2、LCD_y2：LCD第二个坐标点的坐标
  //      触x1、触y1：触摸屏第一个坐标点的坐标
  //      触x2、触y2：触摸屏第二个坐标点的坐标
  LCD_Touch.a_x = (300.0 - 20.0) / (touch_xy[3].x - touch_xy[0].x);
  LCD_Touch.a_x += (20.0 - 300.0)/(touch_xy[2].x - touch_xy[1].x);
  LCD_Touch.a_x /= 2;
  LCD_Touch.c_y = (460.0 - 20.0) / (touch_xy[3].y - touch_xy[0].y);
  LCD_Touch.c_y += (460.0 - 20.0) / (touch_xy[2].y - touch_xy[1].y);
  LCD_Touch.c_y /= 2;
  //获取lcd和触摸屏的xy坐标的偏移量
  //已经求出比例系数，只需取一个区域坐标点(LCD和触摸屏)就可以求偏移量(取两次，求平均值，减小误差)
  //公式: b_x = LCD_x - a_x * 触x
  //      d_y = LCD_y - c_y * 触y
  //备注: b_x、d_y：x、y的偏移量
  //      a_x、c_y：比例系数
  //      LCD_x、LCD_y：LCD一个校准区域坐标点的坐标
  //      触x、触y：触摸屏一个校准区域坐标点的坐标
  LCD_Touch.b_x = 20 - LCD_Touch.a_x * touch_xy[0].x;
  LCD_Touch.b_x += 300 - LCD_Touch.a_x * touch_xy[3].x;
  LCD_Touch.b_x /= 2;
  LCD_Touch.d_y = 20 - LCD_Touch.c_y * touch_xy[0].y;
  LCD_Touch.d_y += 460 - LCD_Touch.c_y * touch_xy[3].y;
  LCD_Touch.d_y /= 2; 
}

/*****************************************************
*函数功能  ：将触摸屏的数值转变为LCD的数值
*函数名    ：LCD_Touch_Transition
*函数参数  ：void
*函数返回值：void
*描述      ：
*        公式：LCD_x = a_x * 触x + b_x
*              LCD_y = c_y * 触y + d_x
*        备注：b_x、d_y：x、y的偏移量
*              a_x、c_y：比例系数
********************************************************/
TOUCH_XY LCD_Touch_Transition(void)
{
  TOUCH_XY touch_xy[2];
  TOUCH_XY xy;
  
  //获取触摸屏坐标
  touch_xy[0] = Touch_Smoothing();
  touch_xy[1] = Touch_Smoothing();
  //将触摸屏坐标转换为LCD屏坐标
  touch_xy[0].x = (u16)(LCD_Touch.a_x * touch_xy[0].x + LCD_Touch.b_x);
  touch_xy[0].y = (u16)(LCD_Touch.c_y * touch_xy[0].y + LCD_Touch.d_y);
  touch_xy[1].x = (u16)(LCD_Touch.a_x * touch_xy[1].x + LCD_Touch.b_x);
  touch_xy[1].y = (u16)(LCD_Touch.c_y * touch_xy[1].y + LCD_Touch.d_y);
  
  //二次滤波并去炸点
  if((abs(touch_xy[0].x - touch_xy[1].x)<8)&&(abs(touch_xy[0].y - touch_xy[1].y)<8))
  {
    xy.x = (touch_xy[0].x + touch_xy[1].x)/2;
    xy.y = (touch_xy[0].y + touch_xy[1].y)/2;
  }
  else
  {
    xy.x = 0xfff;
    xy.y = 0xfff;
  }
  
  return xy;
}





