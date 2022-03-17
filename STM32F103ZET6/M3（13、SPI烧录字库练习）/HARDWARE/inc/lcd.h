#ifndef _LCD_H

  #include "sys.h"
  
    /*******控制线********/
  //片选
  #define LCD_CS_H   	GPIOG->ODR |= (1<<12)  
  #define LCD_CS_L   	GPIOG->ODR &= ~(1<<12) 
  //写使能
  #define LCD_WR_H   	GPIOD->ODR |= (1<<5) 
  #define LCD_WR_L   	GPIOD->ODR &= ~(1<<5) 
  //读使能
  #define LCD_RD_H   	GPIOD->ODR |= (1<<4) 
  #define LCD_RD_L   	GPIOD->ODR &= ~(1<<4) 
  //数据命令选择线
  #define LCD_DC_H   	GPIOG->ODR |= (1<<0) 
  #define LCD_DC_L   	GPIOG->ODR &= ~(1<<0) 
  //背光
  #define LCD_BL_H    GPIOB->ODR |= (1<<0);
  #define LCD_BL_L    GPIOB->ODR &= ~(1<<0);

  #define LCD_DO_0(a)   (a) ? (GPIOD->ODR |= (1<<14)) : (GPIOD->ODR &= ~(1<<14))
  #define LCD_DO_1(a)   (a) ? (GPIOD->ODR |= (1<<15)) : (GPIOD->ODR &= ~(1<<15))
  #define LCD_DO_2(a)   (a) ? (GPIOD->ODR |= (1<<0))  : (GPIOD->ODR &= ~(1<<0))
  #define LCD_DO_3(a)   (a) ? (GPIOD->ODR |= (1<<1))  : (GPIOD->ODR &= ~(1<<1))
  #define LCD_DO_4(a)   (a) ? (GPIOE->ODR |= (1<<7))  : (GPIOE->ODR &= ~(1<<7))
  #define LCD_DO_5(a)   (a) ? (GPIOE->ODR |= (1<<8))  : (GPIOE->ODR &= ~(1<<8))
  #define LCD_DO_6(a)   (a) ? (GPIOE->ODR |= (1<<9))  : (GPIOE->ODR &= ~(1<<9))
  #define LCD_DO_7(a)   (a) ? (GPIOE->ODR |= (1<<10)) : (GPIOE->ODR &= ~(1<<10))
  #define LCD_DO_8(a)   (a) ? (GPIOE->ODR |= (1<<11)) : (GPIOE->ODR &= ~(1<<11))
  #define LCD_DO_9(a)   (a) ? (GPIOE->ODR |= (1<<12)) : (GPIOE->ODR &= ~(1<<12))
  #define LCD_DO_10(a)  (a) ? (GPIOE->ODR |= (1<<13)) : (GPIOE->ODR &= ~(1<<13))
  #define LCD_DO_11(a)  (a) ? (GPIOE->ODR |= (1<<14)) : (GPIOE->ODR &= ~(1<<14))
  #define LCD_DO_12(a)  (a) ? (GPIOE->ODR |= (1<<15)) : (GPIOE->ODR &= ~(1<<15))
  #define LCD_DO_13(a)  (a) ? (GPIOD->ODR |= (1<<8))  : (GPIOD->ODR &= ~(1<<8))
  #define LCD_DO_14(a)  (a) ? (GPIOD->ODR |= (1<<9))  : (GPIOD->ODR &= ~(1<<9))
  #define LCD_DO_15(a)  (a) ? (GPIOD->ODR |= (1<<10)) : (GPIOD->ODR &= ~(1<<10))
  
  //屏幕大小
  #define LCD_Long    240    //长
  #define LCD_Wide    320    //宽 
  //显示方向
  #define LCD_Crosswise  0xA8  //横向
  #define LCD_Lengthways 0x08  //纵向
  
  //颜色
  //GUI颜色
  #define WHITE        0xFFFF
  #define BLACK        0x0000	  
  #define BLUE         0x001F  
  #define BRED         0XF81F
  #define GRED 			   0XFFE0
  #define GBLUE			   0X07FF
  #define RED          0xF800
  #define MAGENTA      0xF81F
  #define GREEN        0x07E0
  #define CYAN         0x7FFF
  #define YELLOW       0xFFE0
  #define BROWN 			 0XBC40 //棕色
  #define BRRED 			 0XFC07 //棕红色
  #define GRAY  			 0X8430 //灰色
  //PANEL的颜色
  #define DARKBLUE     0X01CF	//深蓝色
  #define LIGHTBLUE    0X7D7C	//浅蓝色  
  #define GRAYBLUE     0X5458 //灰蓝色
  #define LIGHTGREEN   0X841F //浅绿色
  #define LIGHTGRAY    0XEF5B //浅灰色(PANNEL)
  #define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
  #define LGRAYBLUE    0XA651 //浅灰蓝色(中间层颜色)
  #define LBBLUE       0X2B12 //浅棕蓝色(选择条目的反色)

  void LCD_IO_Init(void);
  void LCD_ILI9341_CMD(u8 cmd);
  void LCD_ILI9341_Parameter(u16 data);
  void LCD_Clear(u16 colour);
  void LCD_Init(void);
  void LCD_Clear_XY(u16 x,u16 y,u16 Long,u16 Wide,u16 colour);
  void LCD_Point(u16 x,u16 y,u16 colour);
  void LCD_Circle(u16 x,u16 y,u16 r,u16 colour);
  void LCD_Circle2(u16 x,u16 y,u16 r,u16 colour);
  void LCD_Character16(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_Character24(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_Character32(u16 x,u16 y,u8 ch,u16 colour);
  void LCD_String16(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_String24(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_String32(u16 x,u16 y,u8 *str,u16 colour);
  void LCD_Characters_Single32(u16 x,u16 y,u8 *ch,u16 colour);
  void LCD_Characters_String32(u16 x,u16 y,u8 *str,u16 colour);
  
#endif





