#include "lcd.h"
#include "fsmc.h"



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
* @funcName ：LCD_ReadData
* @brief    ：接收LCD屏发来的数据
* @param    ：void
* @retval   ：uint16_t
* @details  ：
* @fn       ：
************************************************************/
uint16_t LCD_ReadData(void)
{
    volatile uint16_t tmp=0;
    tmp = LCD->LCD_DAT;
    return tmp;
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



/******************************************************************************
*函数功能  ：识别屏幕ID型号
*函数名    ：TFTLCD_Id
*函数参数  ：void
*函数返回值：u16
*函数描述  ：
********************************************************************************/
uint16_t LCD_Recognition_Id(void)
{
    uint16_t id = 0;

    /* 尝试9341 ID的读取 */
    LCD->LCD_CMD = 0XD3;
    id = LCD_ReadData();  /* dummy read */
    id = LCD_ReadData();  /* 读到0X00 */
    id = LCD_ReadData();  /* 读取93 */
    id <<= 8;
    id |= LCD_ReadData(); /* 读取41 */

    if (id != 0X9341)    /* 不是 9341 , 尝试看看是不是 ST7789 */
    {
        LCD->LCD_CMD = 0X04;
        id = LCD_ReadData();      /* dummy read */
        id = LCD_ReadData();      /* 读到0X85 */
        id = LCD_ReadData();      /* 读取0X85 */
        id <<= 8;
        id |= LCD_ReadData();     /* 读取0X52 */

        if (id == 0X8552)        /* 将8552的ID转换成7789 */
        {
            id = 0x7789;
        }

        if (id != 0x7789)        /* 也不是ST7789, 尝试是不是 NT35310 */
        {
            LCD->LCD_CMD = 0XD4;
            
            id = LCD_ReadData();  /* dummy read */
            id = LCD_ReadData();  /* 读回0X01 */
            id = LCD_ReadData();  /* 读回0X53 */
            id <<= 8;
            id |= LCD_ReadData(); /* 这里读回0X10 */

            if (id != 0X5310)    /* 也不是NT35310,尝试看看是不是NT35510 */
            {
                /* 发送秘钥（厂家提供,照搬即可） */
                LCD->LCD_CMD = 0xF000;LCD->LCD_DAT = 0x0055;
                LCD->LCD_CMD = 0xF001;LCD->LCD_DAT = 0x00AA;
                LCD->LCD_CMD = 0xF002;LCD->LCD_DAT = 0x0052;
                LCD->LCD_CMD = 0xF003;LCD->LCD_DAT = 0x0008;
                LCD->LCD_CMD = 0xF004;LCD->LCD_DAT = 0x0001;

                LCD->LCD_CMD = 0xC500;     /* 读取ID高8位 */
                id = LCD_ReadData();      /* 读回0X55 */
                id <<= 8;

                LCD->LCD_CMD = 0xC501;     /* 读取ID低8位 */
                id |= LCD_ReadData();     /* 读回0X10 */
                delay_ms(5);
                
                if (id != 0X5510)        /* 也不是NT5510,尝试看看是不是SSD1963 */
                {
                    LCD->LCD_CMD = 0XA1;
                    id = LCD_ReadData();
                    id = LCD_ReadData();  /* 读回0X57 */
                    id <<= 8;
                    id |= LCD_ReadData(); /* 读回0X61 */

                    if (id == 0X5761) id = 0X1963; /* SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963 */
                }
            }
        }
    }

    switch(id)
    {
        case 0X7789: printf("LCD屏型号：ST7789\r\n"); break;
        case 0X9341: printf("LCD屏型号：ILI9341\r\n"); break;
        case 0x5310: printf("LCD屏型号：NT35310\r\n"); break;
        case 0x5510: printf("LCD屏型号：NT35510\r\n"); break;
        case 0X1963: printf("LCD屏型号：SSD1963\r\n"); break;
        default : printf("没有LCD屏或使用了未知型号的LCD屏\r\n"); break;
    }

    return id;
}



