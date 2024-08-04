#include "lcd.h"
#include "fsmc.h"
#include "font.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\lcd.c
*              .\Drivers\BSP\inc\lcd.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：LCD屏(ILI9341)操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* 管理LCD重要参数 */
_lcd_dev lcddev;

/**********************************************************
* @funcName ：LCD_WriteCMD
* @brief    ：向LCD屏发送命令函数
* @param    ：uint8_t cmd(LCD指令/寄存器编号/地址)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_WriteCMD(uint16_t cmd)
{
    LCD->LCD_CMD = cmd;
}



/**********************************************************
* @funcName ：LCD_WriteData
* @brief    ：向LCD屏发送数据
* @param    ：uint16_t data(数据)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_WriteData(uint16_t data)
{
    LCD->LCD_DAT = data;
}



/**********************************************************
* @funcName ：LCD_ILI9341_SetCursor
* @brief    ：设置光标位置(对RGB屏无效)
* @param    ：uint16_t x(x坐标，横坐标)
* @param    ：uint16_t y(y坐标，纵坐标)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_SetCursor(uint16_t x, uint16_t y)
{
    LCD->LCD_CMD = lcddev.setxcmd;  /* 发送命令(x坐标) */
    LCD->LCD_DAT = x >> 8;          /* 写入起始x坐标 */
    LCD->LCD_DAT = x & 0XFF;

    LCD->LCD_CMD = lcddev.setycmd;  /* 发送命令(y坐标) */
    LCD->LCD_DAT = y >> 8;          /* 写入起始y坐标 */
    LCD->LCD_DAT = y & 0XFF;
}



/**********************************************************
* @funcName ：LCD_Clear
* @brief    ：LCD屏幕填充某一种颜色来清屏函数
* @param    ：uint16_t color(颜色数据)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Clear(uint16_t color)
{
    uint32_t i;

    i = lcddev.width * lcddev.height;

    LCD_ILI9341_SetCursor(0x00, 0x00);      /* 设置光标 */

    /* 发送命令(颜色) */
    LCD->LCD_CMD = lcddev.wramcmd;

    /* 开始填色 */
    while(i)
    {
        LCD->LCD_DAT = color;     /* 发送颜色数据 */
        i--;
    }
}



/**********************************************************
* @funcName ：LCD_ILI9341_ScanDir
* @brief    ：设置LCD(ILI9341)的自动扫描方向
* @param    ：uint8_t dir(扫描方向)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_ScanDir(uint8_t dir)
{
    uint16_t regval=0;
    uint8_t dirreg=0;
    uint16_t temp;

    /**
    * 横屏时，对1963不改变扫描方向！其他IC改变扫描方向！
    * 竖屏时，1963改变方向，其他IC不改变扫描方向。
    *
    * 注意：我们这里用的屏幕是ILI9341
    **/
    if(lcddev.dir==1)            /* 横屏时处理 */
    {
        switch(dir)              /* 扫描方向转换 */
        {
            case 0:dir=6;break;
            case 1:dir=7;break;
            case 2:dir=4;break;
            case 3:dir=5;break;
            case 4:dir=1;break;
            case 5:dir=0;break;
            case 6:dir=3;break;
            case 7:dir=2;break;
        }
    }

    /* 根据扫描方式 设置 0X36/0X3600 寄存器 bit 5,6,7 位的值 */
    switch (dir)
    {
        /* 从左到右,从上到下 */
        case L2R_U2D:
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        /* 从左到右,从下到上 */
        case L2R_D2U:
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        /* 从右到左,从上到下 */
        case R2L_U2D:
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        /* 从右到左,从下到上 */
        case R2L_D2U:
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        /* 从上到下,从左到右 */
        case U2D_L2R:
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        /* 从上到下,从右到左 */
        case U2D_R2L:
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        /* 从下到上,从左到右 */
        case D2U_L2R:
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        /* 从下到上,从右到左 */
        case D2U_R2L:
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;    /* 对绝大部分驱动IC, 由0X36寄存器控制 */
    regval |= 0X08;   /* ILI9341要设置BGR位 */

    LCD->LCD_CMD = dirreg;
    LCD->LCD_DAT = regval;

    /* 对屏幕宽高做调整 */
    if(regval&0X20)
    {
        if (lcddev.width < lcddev.height)   /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }
    else  
    {
        if (lcddev.width > lcddev.height)   /* 交换X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }

    LCD->LCD_CMD = lcddev.setxcmd;             /* 发送命令(x坐标) */
    LCD->LCD_DAT = 0;                          /* 写入起始x坐标 */
    LCD->LCD_DAT = 0;
    LCD->LCD_DAT = (lcddev.width - 1) >> 8;    /* 写入结束x坐标 */
    LCD->LCD_DAT = (lcddev.width - 1) & 0XFF;

    LCD->LCD_CMD = lcddev.setycmd;             /* 发送命令(y坐标) */
    LCD->LCD_DAT = 0;                          /* 写入起始y坐标 */
    LCD->LCD_DAT = 0;
    LCD->LCD_DAT = (lcddev.height - 1) >> 8;   /* 写入结束y坐标 */
    LCD->LCD_DAT = (lcddev.height - 1) & 0XFF;
}



/**********************************************************
* @funcName ：LCD_ILI9341_DisplayDir
* @brief    ：设置LCD横竖屏
* @param    ：uint8_t dir
*   @arg    ：0 --- 竖屏   1 --- 横屏
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_DisplayDir(uint8_t dir)
{
    if(dir==0)
    {
        lcddev.dir = 0;             /* 竖屏 */
        lcddev.width = LCD_WIDTH;   /* 屏幕宽 */
        lcddev.height = LCD_HEIGHT; /* 屏幕高 */
    }
    else
    {
        lcddev.dir = 1;             /* 横屏 */
        lcddev.width = LCD_HEIGHT;  /* 屏幕宽 */
        lcddev.height = LCD_WIDTH;  /* 屏幕高 */
    }

    lcddev.wramcmd=0X2C;    /* 颜色指令 */
    lcddev.setxcmd=0X2A;    /* x坐标指令 */
    lcddev.setycmd=0X2B;    /* y坐标指令 */

    LCD_ILI9341_ScanDir(DFT_SCAN_DIR);  /* 默认扫描方向 */
}


 
/**********************************************************
* @funcName ：LCD_ILI9341_Init
* @brief    ：LCD屏幕初始化函数
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_Init(void)
{
    /* FSMC初始化 */
    FSMC_LCD_Init();

    delay1_ms(50);
    lcddev.id = LED_ID;    /* 屏幕型号 */

    /* LCD厂家代码 */
    LCD_WriteCMD(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC1);
    LCD_WriteData(0X30);
    LCD_WriteCMD(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0X12);
    LCD_WriteData(0X81);
    LCD_WriteCMD(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x10);
    LCD_WriteData(0x7A);
    LCD_WriteCMD(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);
    LCD_WriteCMD(0xF7);
    LCD_WriteData(0x20);
    LCD_WriteCMD(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0xC0);        /* Power control */
    LCD_WriteData(0x1B);       /* VRH[5:0] */
    LCD_WriteCMD(0xC1);        /* Power control */
    LCD_WriteData(0x01);       /* SAP[2:0];BT[3:0] */
    LCD_WriteCMD(0xC5);        /* VCM control */
    LCD_WriteData(0x30);       /* 3F */
    LCD_WriteData(0x30);       /* 3C */
    LCD_WriteCMD(0xC7);        /* VCM control2 */
    LCD_WriteData(0XB7);
    LCD_WriteCMD(0x36);        /* Memory Access Control */
    LCD_WriteData(0x48);
    LCD_WriteCMD(0x3A);
    LCD_WriteData(0x55);
    LCD_WriteCMD(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1A);
    LCD_WriteCMD(0xB6);        /*  Display Function Control */
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);
    LCD_WriteCMD(0xF2);        /*  3Gamma Function Disable */
    LCD_WriteData(0x00);
    LCD_WriteCMD(0x26);        /* Gamma curve selected */
    LCD_WriteData(0x01);
    LCD_WriteCMD(0xE0);        /* Set Gamma */
    LCD_WriteData(0x0F);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x28);
    LCD_WriteData(0x08);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x08);
    LCD_WriteData(0x54);
    LCD_WriteData(0XA9);
    LCD_WriteData(0x43);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0XE1);        /* Set Gamma */
    LCD_WriteData(0x00);
    LCD_WriteData(0x15);
    LCD_WriteData(0x17);
    LCD_WriteData(0x07);
    LCD_WriteData(0x11);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x56);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x05);
    LCD_WriteData(0x10);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x0F);
    LCD_WriteCMD(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3f);
    LCD_WriteCMD(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xef);
    LCD_WriteCMD(0x11);        /* Exit Sleep */
    delay_ms(120);
    LCD_WriteCMD(0x29);        /* display on */

    /* 用户自己编写 */
    LCD_ILI9341_DisplayDir(0);    /* 设置屏幕为竖屏 */
    LCD_BL_H;                     /* 背光灯打开 */
    LCD_Clear(WHITE);             /* 清屏函数 */
}



/**********************************************************
* @funcName ：LCD_ILI9341_ClearXY
* @brief    ：LCD屏幕将某个矩形区域某种颜色
* @param    ：uint16_t x      (光标起始位置的x坐标，横坐标)
* @param    ：uint16_t y      (光标起始位置的y坐标，纵坐标)
* @param    ：uint16_t w      (矩形的宽)
* @param    ：uint16_t h      (矩形的高)
* @param    ：uint16_t color (矩形的填充色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_ILI9341_ClearXY(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t i;
    i = w * h;

    LCD_WriteCMD(lcddev.setxcmd);     /* 发送命令(x坐标) */
    /* 写入x坐标数据 */
    LCD_WriteData(x>>8);              /* 起始x坐标 */
    LCD_WriteData(x);
    LCD_WriteData((x+w) >> 8);        /* 末尾x坐标 */
    LCD_WriteData(x+w);

    LCD_WriteCMD(lcddev.setycmd);     /* 发送命令(y坐标) */
    /* 写入y坐标数据 */
    LCD_WriteData(y>>8);              /* 起始y坐标 */
    LCD_WriteData(y);
    LCD_WriteData((y+h) >> 8);        /* 末尾y坐标 */
    LCD_WriteData(y+h);

    LCD_WriteCMD(lcddev.wramcmd);     /* 发送命令(填充颜色) */
    /* 开始填色 */
    while(i)
    {
        LCD_WriteData(color);         /* 发送颜色数据 */
        i--;
    }
}



/**********************************************************
* @funcName ：LCD_Point
* @brief    ：LCD屏幕画点函数
* @param    ：uint16_t x      (光标起始位置的x坐标，横坐标)
* @param    ：uint16_t y      (光标起始位置的y坐标，纵坐标)
* @param    ：uint16_t color (矩形的填充色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_ILI9341_SetCursor(x, y);     /* 设置光标 */

    /* 发送命令(填充颜色) */
    LCD->LCD_CMD = lcddev.wramcmd;

    /* 开始填色 */
    LCD->LCD_DAT = color;
}



/**********************************************************
* @funcName ：LCD_Fill
* @brief    ：在指定区域内填充单个颜色
* @param    ：uint16_t x0      (矩形左上角的x坐标，横坐标)
* @param    ：uint16_t y0      (矩形左上角的y坐标，纵坐标)
* @param    ：uint16_t x1      (矩形右下角的x坐标，横坐标)
* @param    ：uint16_t y1      (矩形右下角的y坐标，纵坐标)
* @param    ：uint16_t color   (矩形的填充色)
* @retval   ：void
* @details  ：
*             矩形区域大小为:(x1 - x0 + 1) * (y1 - y0 + 1)
* @fn       ：
************************************************************/
void LCD_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t i, j;
    uint16_t xlen = x1 - x0 +1;

    for (i = y0; i <= y1; i++)
    {
        LCD_ILI9341_SetCursor(x0, i);      /* 设置光标位置 */

        /* 发送命令(填充颜色) */
        LCD_WriteCMD(lcddev.wramcmd);

        for (j = 0; j < xlen; j++)
        {
            LCD_WriteData(color);          /* 发生颜色数据 */
        }
    }
}



/**********************************************************
* @funcName ：LCD_Color_Fill
* @brief    ：在指定区域内填充指定颜色块(具体用于画图)
* @param    ：uint16_t x0      (矩形左上角的x坐标，横坐标)
* @param    ：uint16_t y0      (矩形左上角的y坐标，纵坐标)
* @param    ：uint16_t x1      (矩形右下角的x坐标，横坐标)
* @param    ：uint16_t y1      (矩形右下角的y坐标，纵坐标)
* @param    ：uint16_t *color  (要填充的颜色数组首地址)
* @retval   ：void
* @details  ：
*             矩形区域大小为:(x1 - x0 + 1) * (y1 - y0 + 1)
* @fn       ：
************************************************************/
void LCD_Color_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = x1 - x0 + 1;            /* 得到填充的宽度 */
    height = y1 - y0 + 1;           /* 高度 */

    for (i = 0; i < height; i++)
    {
        LCD_ILI9341_SetCursor(x0, y0+i);      /* 设置光标位置 */

        /* 发送命令(填充颜色) */
        LCD_WriteCMD(lcddev.wramcmd);

        for (j = 0; j < width; j++)
        {
            LCD_WriteData(color[i*width + j]);   /* 发生颜色数据 */
        }
    }
}



/**********************************************************
* @funcName ：LCD_Draw_Line
* @brief    ：画一条直线(可以是水平线，也可以是斜线)
* @param    ：uint16_t x0      (线前端的x坐标，横坐标)
* @param    ：uint16_t y0      (线前端的y坐标，纵坐标)
* @param    ：uint16_t x1      (线后端的x坐标，横坐标)
* @param    ：uint16_t y1      (线后端的y坐标，纵坐标)
* @param    ：uint16_t color   (线的颜色)
* @retval   ：void
* @details  ：
*             两点确定一条直线
* @fn       ：
************************************************************/
void LCD_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x1 - x0;           /* 计算坐标增量 */
    delta_y = y1 - y0;
    row = x0;
    col = y0;

    if (delta_x > 0)incx = 1;   /* 设置单步方向 */
    else if (delta_x == 0)incx = 0; /* 垂直线 */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* 水平线 */
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* 选取基本增量坐标轴 */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )   /* 画线输出 */
    {
        LCD_Point(row, col, color);        /* 画点 */
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}



/**********************************************************
* @funcName ：LCD_Draw_Hline
* @brief    ：画一条水平直线
* @param    ：uint16_t x     (线前端的x坐标，横坐标)
* @param    ：uint16_t y     (线前端的y坐标，纵坐标)
* @param    ：uint16_t len   (线的长度)
* @param    ：uint16_t color (线的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Draw_Hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcddev.width) || (y > lcddev.height))return;

    LCD_Fill(x, y, x + len - 1, y, color);
}




/**********************************************************
* @funcName ：LCD_Draw_Rectangle
* @brief    ：画空心矩形
* @param    ：uint16_t x0      (矩形左上角的x坐标，横坐标)
* @param    ：uint16_t y0      (矩形左上角的y坐标，纵坐标)
* @param    ：uint16_t x1      (矩形右下角的x坐标，横坐标)
* @param    ：uint16_t y1      (矩形右下角的y坐标，纵坐标)
* @param    ：uint16_t color   (矩形的填充色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Draw_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    LCD_Draw_Line(x0, y0, x1, y0, color);
    LCD_Draw_Line(x0, y0, x0, y1, color);
    LCD_Draw_Line(x0, y1, x1, y1, color);
    LCD_Draw_Line(x1, y0, x1, y1, color);
}



/**********************************************************
* @funcName ：LCD_Draw_Circle
* @brief    ：LCD屏幕画空心圆
* @param    ：uint16_t x      (圆心的x坐标，横坐标)
* @param    ：uint16_t y      (圆心的y坐标，纵坐标)
* @param    ：uint8_t r       (圆心的半径)
* @param    ：uint16_t color (线的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Draw_Circle(uint16_t x, uint16_t y, uint8_t r, uint16_t color)
{
    int a=0, b=r;
    int di = 3 - (r << 1);       /* 判断下个点位置的标志 */

    while (a <= b)
    {
        LCD_Point(x + a, y - b, color);  /* 5 */
        LCD_Point(x + b, y - a, color);  /* 0 */
        LCD_Point(x + b, y + a, color);  /* 4 */
        LCD_Point(x + a, y + b, color);  /* 6 */
        LCD_Point(x - a, y + b, color);  /* 1 */
        LCD_Point(x - b, y + a, color);
        LCD_Point(x - a, y - b, color);  /* 2 */
        LCD_Point(x - b, y - a, color);  /* 7 */
        a++;

        /* 使用Bresenham算法画圆 */
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}



/**********************************************************
* @funcName ：LCD_Fill_Circle
* @brief    ：LCD屏幕画实心圆
* @param    ：uint16_t x      (圆心的x坐标，横坐标)
* @param    ：uint16_t y      (圆心的y坐标，纵坐标)
* @param    ：uint16_t r      (圆心的半径)
* @param    ：uint16_t color  (线的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Fill_Circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;
    uint32_t sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    uint32_t xr = r;

    LCD_Draw_Hline(x - r, y, 2 * r, color);

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            /* draw lines from outside */
            if (xr > imax)
            {
                LCD_Draw_Hline (x - i + 1, y + xr, 2 * (i - 1), color);
                LCD_Draw_Hline (x - i + 1, y - xr, 2 * (i - 1), color);
            }

            xr--;
        }

        /* draw lines from inside (center) */
        LCD_Draw_Hline(x - xr, y + i, 2 * xr, color);
        LCD_Draw_Hline(x - xr, y - i, 2 * xr, color);
    }
}



/**********************************************************
* @funcName ：LCD_Character16
* @brief    ：LCD屏幕显示一个16*16字符
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t ch      (要显示的字符)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Character16(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint8_t temp;

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
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ：LCD_Character24
* @brief    ：LCD屏幕显示一个24*24字符
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t ch      (要显示的字符)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Character24(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint16_t temp;
    uint8_t temp1;
    uint8_t temp2;

    /* 计算出要显示的字符与空格字符的偏移 */
    n = ch - ' ';

    /* 显示 */
    for(i=0;i<24;i++)
    {
        temp1 = model24[n*48+i*2];
        temp2 = model24[n*48+i*2+1];
        temp = (temp1<<4) | (temp2>>4);

        for(j=0;j<12;j++)
        {
            if(temp & (0x800 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ：LCD_Character32
* @brief    ：LCD屏幕显示一个32*32字符
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  ch     (要显示的字符)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Character32(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint16_t temp;
    uint8_t temp1;
    uint8_t temp2;

    /* 计算出要显示的字符与空格字符的偏移 */
    n = ch - ' ';

    /* 显示  */
    for(i=0;i<32;i++)
    {
        temp1 = model32[n*64+i*2];
        temp2 = model32[n*64+i*2+1];
        temp = (temp1<<8) | temp2;

        for(j=0;j<16;j++)
        {
            if(temp & (0x8000 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ：LCD_String16
* @brief    ：LCD屏幕显示一个16*16字符串
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  *str   (要显示的字符串)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_String16(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character16(x,y,*str,color);
        x += 8;
        str++;
        if(x+8 > lcddev.width)
        {
            x = 0;
            y += 16;
        }
    }
}



/**********************************************************
* @funcName ：LCD_String24
* @brief    ：LCD屏幕显示一个24*24字符串
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  *str   (要显示的字符串)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_String24(uint16_t x, uint16_t y, uint8_t *str, uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character24(x,y,*str,color);
        x += 12;
        str++;
        if(x+12 > lcddev.width)
        {
            x = 0;
            y += 24;
        }
    }
}



/**********************************************************
* @funcName ：LCD_String32
* @brief    ：LCD屏幕显示一个32*32字符串
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  *str   (要显示的字符串)
* @param    ：uint16_t color  (字符颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_String32(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character32(x,y,*str,color);
        x += 16;
        str++;
        if(x+16 > lcddev.width)
        {
            x = 0;
            y += 32;
        }
    }
}



/**********************************************************
* @funcName ：LCD_Hanzi32
* @brief    ：LCD屏幕显示一个32*32汉字
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  *ch    (要显示的汉字)
* @param    ：uint16_t color  (汉字颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_LCD_Hanzi32(uint16_t x,uint16_t y,uint8_t *ch,uint16_t color)
{
    uint8_t n = 0;
    uint8_t i,j,k;
    uint32_t temp=0;

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
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ：LCD_HanziStr32
* @brief    ：LCD屏幕显示一个32*32汉字字符串
* @param    ：uint16_t x      (字符左上角的x坐标，横坐标)
* @param    ：uint16_t y      (字符左上角的y坐标，纵坐标)
* @param    ：uint8_t  *str   (要显示的汉字数组)
* @param    ：uint16_t color  (汉字颜色的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_HanziStr32(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_LCD_Hanzi32(x,y,str,color);
        x += 32;
        str+=2;
        if(x+16 > lcddev.width)
        {
            x = 0;
            y += 32;
        }
    }
}



