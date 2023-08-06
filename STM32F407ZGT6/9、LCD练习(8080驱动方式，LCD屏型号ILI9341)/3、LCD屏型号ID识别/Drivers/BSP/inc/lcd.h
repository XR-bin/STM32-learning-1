#ifndef _LCD_H
#define _LCD_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /****************     结构体定义     ****************/
    /* LCD重要参数集 */
    typedef struct  
    {
        uint16_t width;      /* LCD 宽度 */
        uint16_t height;     /* LCD 高度 */
        uint16_t id;         /* LCD ID */
        uint8_t  dir;        /* 横屏还是竖屏控制：0，竖屏；1，横屏 */
        uint16_t wramcmd;    /* 颜色指令 */
        uint16_t setxcmd;    /* 设置x坐标指令 */
        uint16_t setycmd;    /* 设置y坐标指令 */
    }_lcd_dev; 



    /********************   宏定义   ********************/
    /*** 控制线 ***/
    /* 片选 */
    #define LCD_CS_H    GPIOG->ODR |= (1<<12)      /* 片选失能 */
    #define LCD_CS_L    GPIOG->ODR &= ~(1<<12)     /* 片选使能 */
    /* 写使能 */
    #define LCD_WR_H    GPIOD->ODR |= (1<<5) 
    #define LCD_WR_L    GPIOD->ODR &= ~(1<<5)      /* 写使能 */
    /* 读使能 */
    #define LCD_RD_H    GPIOD->ODR |= (1<<4) 
    #define LCD_RD_L    GPIOD->ODR &= ~(1<<4)      /* 读使能 */
    /* 数据命令选择线 */
    #define LCD_DC_H    GPIOF->ODR |= (1<<12)      /* 收发数据 */
    #define LCD_DC_L    GPIOF->ODR &= ~(1<<12)     /* 收发命令 */
    /* 背光 */
    #define LCD_BL_H    GPIOB->ODR |= (1<<15)      /* 开启背光 */
    #define LCD_BL_L    GPIOB->ODR &= ~(1<<15)     /* 关闭背光 */

    /*** 数据线 ***/
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

    /*** 屏幕的基础信息 ***/
    /* 屏幕型号 */
    #define LED_ID        9341   /* 9341型号屏幕 */

    /* 屏幕大小 */
    #define LCD_WIDTH     240    /* 宽 */
    #define LCD_HEIGHT    320    /* 高 */

    /* 显示方向 */
    #define LCD_Crosswise  0xA8  /* 横向 */
    #define LCD_Lengthways 0x08  /* 纵向 */
  
    /* 扫描方向定义 */
    #define L2R_U2D  0 /* 从左到右,从上到下 */
    #define L2R_D2U  1 /* 从左到右,从下到上 */
    #define R2L_U2D  2 /* 从右到左,从上到下 */
    #define R2L_D2U  3 /* 从右到左,从下到上 */

    #define U2D_L2R  4 /* 从上到下,从左到右 */
    #define U2D_R2L  5 /* 从上到下,从右到左 */
    #define D2U_L2R  6 /* 从下到上,从左到右 */
    #define D2U_R2L  7 /* 从下到上,从右到左 */
    
    #define DFT_SCAN_DIR  L2R_U2D  /* 默认的扫描方向 */

    /*** 颜色 ***/
    /* 常用画笔颜色 */
    #define WHITE           0xFFFF      /* 白色 */
    #define BLACK           0x0000      /* 黑色 */
    #define RED             0xF800      /* 红色 */
    #define GREEN           0x07E0      /* 绿色 */
    #define BLUE            0x001F      /* 蓝色 */ 
    #define MAGENTA         0XF81F      /* 品红色/紫红色 = BLUE + RED */
    #define YELLOW          0XFFE0      /* 黄色 = GREEN + RED */
    #define CYAN            0X07FF      /* 青色 = GREEN + BLUE */  

    /* 非常用颜色 */
    #define BROWN           0XBC40      /* 棕色 */
    #define BRRED           0XFC07      /* 棕红色 */
    #define GRAY            0X8430      /* 灰色 */ 
    #define DARKBLUE        0X01CF      /* 深蓝色 */
    #define LIGHTBLUE       0X7D7C      /* 浅蓝色 */ 
    #define GRAYBLUE        0X5458      /* 灰蓝色 */ 
    #define LIGHTGREEN      0X841F      /* 浅绿色 */  
    #define LGRAY           0XC618      /* 浅灰色(PANNEL),窗体背景色 */ 
    #define LGRAYBLUE       0XA651      /* 浅灰蓝色(中间层颜色) */ 
    #define LBBLUE          0X2B12      /* 浅棕蓝色(选择条目的反色) */ 



    /****************    变量外部声明   *****************/
    extern _lcd_dev lcddev;



    /****************    函数外部声明   *****************/
    /* 受屏幕型号影响 */
    void LCD_ILI9341_Init(void);                          /* 屏幕初始化 */
    void LCD_ILI9341_SetCursor(uint16_t x, uint16_t y);   /* 设置光标位置 */
    void LCD_ILI9341_ScanDir(uint8_t dir);                /* 设置LCD(ILI9341)的自动扫描方向 */
    void LCD_ILI9341_DisplayDir(uint8_t dir);             /* 设置LCD横竖屏 */

    /* 不受屏幕型号影响 */
    void LCD_IO_Init(void);                         /* LCD相关引脚初始化 */
    void Output_Mode(void);                         /* 切换为输出模式 */
    void Input_Mode(void);                          /* 切换为输入模式 */
    void LCD_WriteCMD(uint16_t cmd);                /* 向LCD发生命令 */
    void LCD_WriteData(uint16_t data);              /* 向LCD发生数据 */
    uint16_t LCD_ReadData(void);                    /* 接收LCD发来的数据 */
    uint16_t LCD_Recognition_Id(void);              /* 识别LCD屏的型号 */

    /* 画图操作 */
    void LCD_Clear(uint16_t color);                 /* LCD清屏 */

#endif





