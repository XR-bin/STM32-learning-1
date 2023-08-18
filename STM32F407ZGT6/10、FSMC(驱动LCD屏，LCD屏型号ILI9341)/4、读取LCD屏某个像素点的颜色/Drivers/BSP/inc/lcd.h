#ifndef __LCD_H
#define __LCD_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



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
    /* 背光 */
    #define LCD_BL_H    GPIOB->ODR |= (1<<15)       /* 开启背光 */
    #define LCD_BL_L    GPIOB->ODR &= ~(1<<15)      /* 关闭背光 */

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
    void LCD_ILI9341_ClearXY(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);  /* LCD屏幕将某个矩形区域某种颜色 */

    /* 不受屏幕型号影响 */
    void LCD_WriteCMD(uint16_t cmd);                /* 向LCD发生命令 */
    void LCD_WriteData(uint16_t data);              /* 向LCD发生数据 */
    uint16_t LCD_ReadData(void);                    /* 接收LCD发来的数据 */
    uint32_t LCD_ReadPoint(uint16_t x, uint16_t y); /* 读取某个像素点的颜色 */

    /* 画图操作 */
    void LCD_Clear(uint16_t color);                 /* LCD清屏 */

#endif





