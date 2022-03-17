#include "sys.h"
#include "lcd.h"
#include "delay.h"
#include "font.h"

//我们知道ARGB指的是一种色彩模式，里面A代表Alpha，R表示red，G表示green，B表示blue。
typedef struct _HEADCOLOR
{
   unsigned char scan;      //扫描方向
   unsigned char gray;      //灰度值
   unsigned short w;        //图片宽
   unsigned short h;        //图片高
   unsigned char is565;     //每个像素占四位，即R=5，G=6，B=5，没有透明度，那么一个像素点占5+6+5=16位
   unsigned char rgb;       //RGB
}HEADCOLOR;

/****************************************************************************
*函数功能  ：对LCD对应的GPIO口进行初始化设置
*函数名    ：LCD_IO_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*     数据线：//通用输出 推挽      速度50M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     控制线：//通用输出  推挽      速度50M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(数据/命名)---PG0	 
*           BL（背光）--PB0   
*     整理：
*         PB：0
*         PD：0-1  4-5  8-10  14-15  
*         PE：7-15
*         PG：0  12          
***************************************************************************/
void LCD_IO_Init(void)
{
  //PB
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<3;
  //端口配置寄存器
  GPIOB->CRL &= ~(0xf);
  GPIOB->CRL |= (0x3);
  //PD 
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<5;
  //端口配置寄存器
  GPIOD->CRL &= ~(0xff00ff);
  GPIOD->CRH &= ~(0xff000fff);
  GPIOD->CRL |= (0x330033);
  GPIOD->CRH |= (0x33000333);
  //PE
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<6;
  //端口配置寄存器
  GPIOE->CRL &= ~(0xf0000000);
  GPIOE->CRH &= ~(0xffffffff);
  GPIOE->CRL |= (0x30000000);
  GPIOE->CRH |= (0x33333333);
  //PG
  //GPIOx时钟使能
  RCC->APB2ENR |= 1<<8;
  //端口配置寄存器
  GPIOG->CRL &= ~(0xf);
  GPIOG->CRH &= ~(0x000f0000);
  GPIOG->CRL |= (0x3);
  GPIOG->CRH |= (0x00030000);
  
  //片选拉高
  LCD_CS_H;
  //写使能拉高
  LCD_WR_H;
  //读使能拉高
  LCD_RD_H;
  //数据命令选择线拉高
  LCD_DC_H;
  //背光拉低(关闭)
  LCD_BL_L;
}

/***************************************************************************************
*函数功能  ：主控芯片发送命令函数
*函数名    ：LCD_ILI9341_CMD
*函数参数  ：u8 cmd    
*函数返回值：void
*函数描述  ：  
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*****************************************************************************************/
void LCD_ILI9341_CMD(u8 cmd)
{
  //读使能拉高
  LCD_RD_H;
  //数据命令选择线拉低
  LCD_DC_L;
  //片选拉低
  LCD_CS_L;
  //写使能拉低
  LCD_WR_L;
  //准备要发送的命令
  if(cmd & 0x01) LCD_DO_0(1);    else LCD_DO_0(0);
	if(cmd & 0x02) LCD_DO_1(1);    else LCD_DO_1(0);
	if(cmd & 0x04) LCD_DO_2(1);    else LCD_DO_2(0);
	if(cmd & 0x08) LCD_DO_3(1);    else LCD_DO_3(0);
	if(cmd & 0x10) LCD_DO_4(1);    else LCD_DO_4(0);
	if(cmd & 0x20) LCD_DO_5(1);    else LCD_DO_5(0);
	if(cmd & 0x40) LCD_DO_6(1);    else LCD_DO_6(0);
	if(cmd & 0x80) LCD_DO_7(1);    else LCD_DO_7(0);
  //写使能拉高
  LCD_WR_H;
  //片选拉高
  LCD_CS_H;
}

/***************************************************************************************
*函数功能  ：主控芯片发送数据函数
*函数名    ：LCD_ILI9341_Parameter
*函数参数  ：u16  data   
*函数返回值：void
*函数描述  ：	
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*****************************************************************************************/
void LCD_ILI9341_Parameter(u16 data)
{
  //读使能拉高
  LCD_RD_H;
  //数据命令选择线拉低
  LCD_DC_H;
  //片选拉低
  LCD_CS_L;
  //写使能拉低
  LCD_WR_L;
  //准备要发送的命令
  if(data & 0x0001)  LCD_DO_0(1);    else LCD_DO_0(0);
	if(data & 0x0002)  LCD_DO_1(1);    else LCD_DO_1(0);
	if(data & 0x0004)  LCD_DO_2(1);    else LCD_DO_2(0);
	if(data & 0x0008)  LCD_DO_3(1);    else LCD_DO_3(0);
	if(data & 0x0010)  LCD_DO_4(1);    else LCD_DO_4(0);
	if(data & 0x0020)  LCD_DO_5(1);    else LCD_DO_5(0);
	if(data & 0x0040)  LCD_DO_6(1);    else LCD_DO_6(0);
	if(data & 0x0080)  LCD_DO_7(1);    else LCD_DO_7(0);
	if(data & 0x0100)  LCD_DO_8(1);    else LCD_DO_8(0);
	if(data & 0x0200)  LCD_DO_9(1);    else LCD_DO_9(0);
	if(data & 0x0400)  LCD_DO_10(1);   else LCD_DO_10(0);
	if(data & 0x0800)  LCD_DO_11(1);   else LCD_DO_11(0);
	if(data & 0x1000)  LCD_DO_12(1);   else LCD_DO_12(0);
	if(data & 0x2000)  LCD_DO_13(1);   else LCD_DO_13(0);
	if(data & 0x4000)  LCD_DO_14(1);   else LCD_DO_14(0);
	if(data & 0x8000)  LCD_DO_15(1);   else LCD_DO_15(0);
  //写使能拉高
  LCD_WR_H;
  //片选拉高
  LCD_CS_H;
}

/**************************************************************************
*函数功能  ：LCD屏幕清屏函数
*函数名    ：LCD_Clear
*函数参数  ：u16 colour
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Clear(u16 colour)
{
  u32 i;
  
  i = LCD_Long * LCD_Wide;
  
  //发送命令(x坐标)
  LCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  //末尾x坐标
  LCD_ILI9341_Parameter((LCD_Long-1) >> 8);
  LCD_ILI9341_Parameter(LCD_Long-1);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(0);
  LCD_ILI9341_Parameter(0);
  //末尾y坐标
  LCD_ILI9341_Parameter((LCD_Wide-1) >> 8);
  LCD_ILI9341_Parameter(LCD_Wide-1);
  
  //发送命令(颜色)
  LCD_ILI9341_CMD(0x2c);
  //开始填色
  while(i)
  {
    LCD_ILI9341_Parameter(colour);
    i--;
  }
}

/***************************************************************************************
*函数功能  ：LCD屏幕初始化函数
*函数名    ：LCD_Init
*函数参数  ：void   
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************************/
void LCD_Init(void)
{
  /*IO口初始化*/
	LCD_IO_Init();
  
  /*LCD厂家代码*/
  	LCD_ILI9341_CMD(0xCF);  
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0xC1); 
		LCD_ILI9341_Parameter(0X30); 
		LCD_ILI9341_CMD(0xED);  
	  LCD_ILI9341_Parameter(0x64); 
		LCD_ILI9341_Parameter(0x03); 
		LCD_ILI9341_Parameter(0X12); 
		LCD_ILI9341_Parameter(0X81); 
		LCD_ILI9341_CMD(0xE8);  
		LCD_ILI9341_Parameter(0x85); 
		LCD_ILI9341_Parameter(0x10); 
		LCD_ILI9341_Parameter(0x7A); 
		LCD_ILI9341_CMD(0xCB);  
		LCD_ILI9341_Parameter(0x39); 
		LCD_ILI9341_Parameter(0x2C); 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x34); 
		LCD_ILI9341_Parameter(0x02); 
		LCD_ILI9341_CMD(0xF7);  
		LCD_ILI9341_Parameter(0x20); 
		LCD_ILI9341_CMD(0xEA);  
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_CMD(0xC0);    //Power control 
		LCD_ILI9341_Parameter(0x1B);   //VRH[5:0] 
		LCD_ILI9341_CMD(0xC1);    //Power control 
		LCD_ILI9341_Parameter(0x01);   //SAP[2:0];BT[3:0] 
		LCD_ILI9341_CMD(0xC5);    //VCM control 
		LCD_ILI9341_Parameter(0x30); 	 //3F
		LCD_ILI9341_Parameter(0x30); 	 //3C
		LCD_ILI9341_CMD(0xC7);    //VCM control2 
		LCD_ILI9341_Parameter(0XB7); 
		LCD_ILI9341_CMD(0x36);    // Memory Access Control 
		LCD_ILI9341_Parameter(0x48); 
		LCD_ILI9341_CMD(0x3A);   
		LCD_ILI9341_Parameter(0x55); 
		LCD_ILI9341_CMD(0xB1);   
		LCD_ILI9341_Parameter(0x00);   
		LCD_ILI9341_Parameter(0x1A); 
		LCD_ILI9341_CMD(0xB6);    // Display Function Control 
		LCD_ILI9341_Parameter(0x0A); 
		LCD_ILI9341_Parameter(0xA2); 
		LCD_ILI9341_CMD(0xF2);    // 3Gamma Function Disable 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_CMD(0x26);    //Gamma curve selected 
		LCD_ILI9341_Parameter(0x01); 
		LCD_ILI9341_CMD(0xE0);    //Set Gamma 
		LCD_ILI9341_Parameter(0x0F); 
		LCD_ILI9341_Parameter(0x2A); 
		LCD_ILI9341_Parameter(0x28); 
		LCD_ILI9341_Parameter(0x08); 
		LCD_ILI9341_Parameter(0x0E); 
		LCD_ILI9341_Parameter(0x08); 
		LCD_ILI9341_Parameter(0x54); 
		LCD_ILI9341_Parameter(0XA9); 
		LCD_ILI9341_Parameter(0x43); 
		LCD_ILI9341_Parameter(0x0A); 
		LCD_ILI9341_Parameter(0x0F); 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x00); 		 
		LCD_ILI9341_CMD(0XE1);    //Set Gamma 
		LCD_ILI9341_Parameter(0x00); 
		LCD_ILI9341_Parameter(0x15); 
		LCD_ILI9341_Parameter(0x17); 
		LCD_ILI9341_Parameter(0x07); 
		LCD_ILI9341_Parameter(0x11); 
		LCD_ILI9341_Parameter(0x06); 
		LCD_ILI9341_Parameter(0x2B); 
		LCD_ILI9341_Parameter(0x56); 
		LCD_ILI9341_Parameter(0x3C); 
		LCD_ILI9341_Parameter(0x05); 
		LCD_ILI9341_Parameter(0x10); 
		LCD_ILI9341_Parameter(0x0F); 
		LCD_ILI9341_Parameter(0x3F); 
		LCD_ILI9341_Parameter(0x3F); 
		LCD_ILI9341_Parameter(0x0F); 
		LCD_ILI9341_CMD(0x2B); 
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x01);
		LCD_ILI9341_Parameter(0x3f);
		LCD_ILI9341_CMD(0x2A); 
    LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0x00);
		LCD_ILI9341_Parameter(0xef);	
    
    LCD_ILI9341_CMD(0x36);//控制横屏还是竖屏，xy(0,0)中心点是左上、右上、左下、右下
    LCD_ILI9341_Parameter(LCD_Lengthways); //纵向显示，(0,0)在左上角
    
		LCD_ILI9341_CMD(0x11); //Exit Sleep
		delay_ms(120);
		LCD_ILI9341_CMD(0x29); //display on	

  
  /*用户自己编写*/
	//背光灯打开
	LCD_BL_H;
	//清屏函数
	LCD_Clear(WHITE);
}

/**************************************************************************
*函数功能  ：LCD屏幕将某个矩形区域某种颜色
*函数名    ：LCD_Clear_XY
*函数参数  ：u16 x,u16 y,u16 w,u16 h,u16 colour  
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour)
{
  u32 i;
  i = Long * Wide;
  //发送命令(x坐标)
  LCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(x>>8);
  LCD_ILI9341_Parameter(x);
  //末尾x坐标
  LCD_ILI9341_Parameter((x+Long) >> 8);
  LCD_ILI9341_Parameter(x+Long);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(y>>8);
  LCD_ILI9341_Parameter(y);
  //末尾y坐标
  LCD_ILI9341_Parameter((y+Wide) >> 8);
  LCD_ILI9341_Parameter(y+Wide);
  
  //发送命令(颜色)
  LCD_ILI9341_CMD(0x2c);
  //开始填色
  while(i)
  {
    LCD_ILI9341_Parameter(colour);
    i--;
  }
}

/**************************************************************************
*函数功能  ：LCD屏幕画点函数
*函数名    ：LCD_Point
*函数参数  ：u16 x,u16 y,u16 colour 
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
*****************************************************************************/
void LCD_Point(u16 x,u16 y,u16 colour)
{
  //发送命令(x坐标)
  LCD_ILI9341_CMD(0x2a);
  //写入x坐标数据
  //起始x坐标
  LCD_ILI9341_Parameter(x >> 8);
  LCD_ILI9341_Parameter(x);
  //末尾x坐标
  LCD_ILI9341_Parameter(x >> 8);
  LCD_ILI9341_Parameter(x);
  
  //发送命令(y坐标)
  LCD_ILI9341_CMD(0x2b);
  //写入y坐标数据
  //起始y坐标
  LCD_ILI9341_Parameter(y >> 8);
  LCD_ILI9341_Parameter(y);
  //末尾y坐标
  LCD_ILI9341_Parameter(y >> 8);
  LCD_ILI9341_Parameter(y);
  
  //发送命令(颜色)
  LCD_ILI9341_CMD(0x2c);
  //开始填色
  LCD_ILI9341_Parameter(colour);
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(空心)
*函数名    ：LCD_Circle
*函数参数  ：u16 x,u16 y,u16 r,u16 colour 
*函数返回值：void
*函数描述  ：			
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void LCD_Circle(u16 x,u16 y,u16 r,u16 colour)
{
  u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y) >= (r)*(r-2) && (i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				LCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕画圆函数(实心圆)
*函数名    ：LCD_Circle2
*函数参数  ：u16 x,u16 y,u16 r,u16 colour  颜色
*函数返回值：void
*函数描述  ：
*         片选             LCD_CS_H   LCD_CS_L   
*         写使能           LCD_WR_H   LCD_WR_L   
*         读使能           LCD_RD_H   LCD_RD_L 
*         数据命令选择线   LCD_DC_H   LCD_DC_L
*         背光             LCD_BL_H   LCD_BL_L  
*         横坐标数据命令   0x2a
*         纵坐标数据命名   0x2b
*         颜色数据命令     0x2c
********************************************************************************/
void LCD_Circle2(u16 x,u16 y,u16 r,u16 colour)
{
	u16 i,j;
	
	for(i=x-r;i<=x+r;i++)
	{
		for(j=y-r;j<=y+r;j++)
		{
			if((i-x)*(i-x) + (j-y)*(j-y)<=r*r)
			{
				LCD_Point(i,j,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个16*16字符
*函数名    ：LCD_ Character16
*函数参数  ：u16 x,u16 y,u8 ch,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Character16(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u8 temp;
	
	/*计算出要显示的字符与空格字符的偏移*/
	n = ch - ' ';
	
	/*显示*/
	for(i=0;i<16;i++)
	{
		temp = model16[n*16+i];
		for(j=0;j<8;j++)
		{
			if(temp & (0x80 >> j))
			{
				LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个24*24字符
*函数名    ：LCD_ Character24
*函数参数  ：u16 x,u16 y,u8 ch,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Character24(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u16 temp;
  u8 temp1;
  u8 temp2;
	
	/*计算出要显示的字符与空格字符的偏移*/
	n = ch - ' ';
	
	/*显示*/
	for(i=0;i<24;i++)
	{
		temp1 = model24[n*48+i*2];
    temp2 = model24[n*48+i*2+1];
    temp = (temp1<<4) | (temp2>>4);
		for(j=0;j<12;j++)
		{
			if(temp & (0x800 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个32*32字符
*函数名    ：LCD_ Character32
*函数参数  ：u16 x,u16 y,u8 ch,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Character32(u16 x,u16 y,u8 ch,u16 colour)
{
  u8 n;
	u8 i,j;
	u16 temp;
  u8 temp1;
  u8 temp2;
	
	/*计算出要显示的字符与空格字符的偏移*/
	n = ch - ' ';
	
	/*显示*/
	for(i=0;i<32;i++)
	{
		temp1 = model32[n*64+i*2];
    temp2 = model32[n*64+i*2+1];
    temp = (temp1<<8) | temp2;
		for(j=0;j<16;j++)
		{
			if(temp & (0x8000 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个16*16字符串
*函数名    ：LCD_String16
*函数参数  ：u16 x,u16 y,u8 *str,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_String16(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character16(x,y,*str,colour);
    x += 8;
    str++;
    if(x+8>LCD_Long)
    {
      x = 0;
      y += 16;
    }
  }
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个24*24字符串
*函数名    ：LCD_String24
*函数参数  ：u16 x,u16 y,u8 *str,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_String24(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character24(x,y,*str,colour);
    x += 12;
    str++;
    if(x+12>LCD_Long)
    {
      x = 0;
      y += 24;
    }
  }
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个32*32字符串
*函数名    ：LCD_String32
*函数参数  ：u16 x,u16 y,u8 *str,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_String32(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Character32(x,y,*str,colour);
    x += 16;
    str++;
    if(x+16>LCD_Long)
    {
      x = 0;
      y += 32;
    }
  }
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个32*32汉字
*函数名    ：LCD_Characters_Single32
*函数参数  ：u16 x,u16 y,u8 *ch,u16 colour
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Characters_Single32(u16 x,u16 y,u8 *ch,u16 colour)
{
  u8 n = 0;
	u8 i,j,k;
	u32 temp=0;

  
  while(1)
  {
    if((*ch == query[n*2])&&(*(ch+1)==query[n*2+1]))
    {
      break;
    }
    n++;
  }
  for(i=0;i<32;i++)
  {
    for(k=0;k<4;k++)
    {
      temp<<=8;
      temp |= hanzi[16*8*n+k+i*4];
    }
    for(j=0;j<32;j++)
		{
			if(temp & (0x80000000 >> j))
			{
        LCD_Point(x+j,y+i,colour);
			}
		}
  }
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一个32*32汉字字符串
*函数名    ：LCD_Characters_String32
*函数参数  ：u16 x,u16 y,u8 *str,u16 colour 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Characters_String32(u16 x,u16 y,u8 *str,u16 colour)
{
  while(*str != '\0')
  {
    LCD_Characters_Single32(x,y,str,colour);
    x += 32;
    str+=2;
    if(x+16>LCD_Long)
    {
      x = 0;
      y += 32;
    }
  }
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一张图片(使用与低位在前的图片模组个是，例如tu.c)
*函数名    ：LCD_Picture
*函数参数  ：u16 x,u16 y,u8 *buf 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Picture(u16 x,u16 y,u8 *buf)
{
  u32 i;
  
  HEADCOLOR *p;
  u16 *k;
  
  p = (HEADCOLOR *)buf;
  
  k = (u16 *)(buf+8);  //强制转换这里有个问题得注意：强转是将后一位作高位，前一位做低位
  i = p->w * p->h;     //例如：u8 str[2]={0x22,0x32}----强转----->u16 a = 0x3222
	                     //例如：u16 a = 0x1234  ------强转------>u8 *a    打印出来的顺序是 0x34  0x12
	/*确定横坐标*/
	//发送横坐标命令(X)  0x2A
	LCD_ILI9341_CMD(0x2A);
	//起始横坐标
	LCD_ILI9341_Parameter(x >> 8);          //发送高八位
	LCD_ILI9341_Parameter(x);               //发送低八位
	//结束横坐标
	LCD_ILI9341_Parameter((x+(p->w)-1) >> 8); //发送高八位
	LCD_ILI9341_Parameter(x+(p->w)-1);        //发送低八位
	
	/*确定纵坐标*/    
	//发送纵坐标命令(Y)  0x2B
	LCD_ILI9341_CMD(0x2B);
	//起始纵坐标
	LCD_ILI9341_Parameter(y>>8);            //发送高八位
  LCD_ILI9341_Parameter(y);               //发送低八位
	//结束纵坐标
	LCD_ILI9341_Parameter((y+(p->h)-1) >> 8); //发送高八位
	LCD_ILI9341_Parameter(y+(p->h)-1);        //发送低八位
	
	/*确定颜色*/
	//发送颜色命令
	LCD_ILI9341_CMD(0x2C);
	//发送颜色数据 
	while(i)
	{
    LCD_ILI9341_Parameter(*k);
    k++;
    i--;
	}
}

/******************************************************************************
*函数功能  ：LCD屏幕显示一张图片(使用与高位在前的图片模组个是，例如tu2.c)
*函数名    ：LCD_Picture2
*函数参数  ：u16 x,u16 y,u8 *buf 
*函数返回值：void
*函数描述  ：					
********************************************************************************/
void LCD_Picture2(u16 x,u16 y,u8 *buf)
{
  u32 i;
  u32 k = 0;
  
  i = LCD_Long * LCD_Wide;
	
	/*确定横坐标*/
	//发送横坐标命令(X)  0x2A
	LCD_ILI9341_CMD(0x2A);
	//起始横坐标
	LCD_ILI9341_Parameter(x >> 8);          //发送高八位
	LCD_ILI9341_Parameter(x);               //发送低八位
	//结束横坐标
	LCD_ILI9341_Parameter((x+(320)-1) >> 8); //发送高八位
	LCD_ILI9341_Parameter(x+(320)-1);        //发送低八位
	
	/*确定纵坐标*/    
	//发送纵坐标命令(Y)  0x2B
	LCD_ILI9341_CMD(0x2B);
	//起始纵坐标
	LCD_ILI9341_Parameter(y>>8);            //发送高八位
  LCD_ILI9341_Parameter(y);               //发送低八位
	//结束纵坐标
	LCD_ILI9341_Parameter((y+(480)-1) >> 8); //发送高八位
	LCD_ILI9341_Parameter(y+(480)-1);        //发送低八位
	
	/*确定颜色*/
	//发送颜色命令
	LCD_ILI9341_CMD(0x2C);
	//发送颜色数据 
	while(i)
	{
    LCD_ILI9341_Parameter((buf[k*2]<<8)|(buf[k*2+1]));
    k++;
    i--;
	}
}






