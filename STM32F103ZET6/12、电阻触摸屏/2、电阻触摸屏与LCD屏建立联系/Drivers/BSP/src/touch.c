#include "touch.h"
#include "lcd.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-03
* @brief     ：电阻触摸屏驱动配置和操作代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



_m_tp_dev tp_dev =
{
    Touch_Init,               /* 触摸屏初始化 */
    LCD_Touch_Calibration,    /* 触摸屏校准 */
    Touch_Scan,               /* 获取触摸坐标 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};




/**********************************************************
* @funcName ：Touch_IO_Init
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            CS     PF11  推挽输出
*            SCK    PB1   推挽输出
*            PEN    PF10  通用输入
*            MISO   PB2   通用输入
*            MOSI   PF9   推挽输出
* @fn       ：
************************************************************/
void Touch_IO_Init(void)
{
    /* PB */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= 1<<3;
    /* 端口配置寄存器 */
    GPIOB->CRL &= ~((0xf<<(4*1))|(0xf<<(4*2)));
    GPIOB->CRL |= ((3<<(4*1))|(4<<(4*2)));

    /* PF */
    /* GPIOx时钟使能 */
    RCC->APB2ENR |= 1<<7;
    /* 端口配置寄存器 */
    GPIOF->CRH &= ~((0xf<<(4*(9-8)))|(0xf<<(4*(10-8)))|(0xf<<(4*(11-8))));
    GPIOF->CRH |= ((3<<(4*(9-8)))|(4<<(4*(10-8)))|(3<<(4*(11-8))));

    /* 初始化状态空闲 */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ：Touch_Data
* @brief    ：对电阻触摸屏芯片对应的GPIO口进行初始化设置
* @param    ：uint8_t command (指令)
* @retval   ：uint16_t
* @details  ：
*            片选线        T_CS_H       T_CS_L  
*            时钟线        T_SCK_H      T_SCK_L  
*            输出数据线    T_MOSI_H     T_MOSI_L
*            输入数据线    T_MISO
*            笔接触线      T_PEN
* @fn       ：
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;                        /* 数据线拉低 */
    T_CS_L;                          /* 片选使能 */

    for(i=0;i<8;i++)
    {
        T_SCK_L;                     /* 时钟线拉低 */

        /* 判断指令位电平 */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;               /* 指令数据左移 */

        T_SCK_H;                     /* 时钟线拉高 */
        delay_us(1);                 /* 延时，用于从机读取数据 */
    }
    T_SCK_L;                         /* 时钟线拉低 */
    T_MOSI_L;                        /* 数据线拉低 */
    delay_us(6);                     /* 延时6us，用于等待ADC把数据转换好 */

    /* 空出一个空脉冲 */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;                  /* 指令数据左移 */
        T_SCK_L;                     /* 时钟线拉低 */
        delay_us(1);                 /* 延时，用于数据建立 */
        T_SCK_H;                     /* 时钟线拉高 */
        if(T_MISO) data |= 0x1;      /* 读取数据位电平 */
        delay_us(1);                 /* 延时，用于数据建立 */
    }

    T_CS_H;                          /* 片选拉高 */
    T_SCK_L;                         /* 时钟线拉低 */

    return data;
}



/**********************************************************
* @funcName ：Touch_Smoothing
* @brief    ：电阻触摸屏数据滤波函数
* @param    ：void
* @retval   ：TOUCH_XY
* @details  ：
* @fn       ：
************************************************************/
TOUCH_XY Touch_Smoothing(void)
{
    uint8_t i,j;
    uint16_t touch_x[TP_READ_TIMES];
    uint16_t touch_y[TP_READ_TIMES];
    uint16_t sum_x = 0,sum_y = 0;
    TOUCH_XY mean;

    /* 获取TP_READ_TIMES次坐标数据 */
    for(i=0;i<TP_READ_TIMES;i++)
    {
        touch_x[i] = Touch_Data(tp_dev.comX);
        touch_y[i] = Touch_Data(tp_dev.comY);
    }

    /* 对数据进行升序排序(冒泡排序) */
    for(i=1; i<TP_READ_TIMES; i++)
    {
        for(j=0; j<TP_READ_TIMES-i; j++)
        {
            /* x坐标数据 */
            if(touch_x[j] > touch_x[j+1])
            {
                touch_x[j] ^= touch_x[j+1];
                touch_x[j+1] ^= touch_x[j];
                touch_x[j] ^= touch_x[j+1];
            }

            /* y坐标数据 */
            if(touch_y[j]>touch_y[j+1])
            {
                touch_y[j] ^= touch_y[j+1];
                touch_y[j+1] ^= touch_y[j];
                touch_y[j] ^= touch_y[j+1];
            }
        }
    }

    /* 去掉TP_LOST_VAL个最大和最小，取平均值 */
    for(i=TP_LOST_VAL; i<TP_READ_TIMES - TP_LOST_VAL; i++)
    {
        sum_x += touch_x[i];
        sum_y += touch_y[i];
    }

    mean.x = sum_x / (TP_READ_TIMES - TP_LOST_VAL);
    mean.y = sum_y / (TP_READ_TIMES - TP_LOST_VAL);

    return mean;
}



/**********************************************************
* @funcName ：用于校准的十字提示函数
* @brief    ：LCD_Calibration_Cross
* @param    ：uint16_t x     (十字的中心点x坐标)
* @param    ：uint16_t y     (十字的中心点y坐标)
* @param    ：uint16_t color (十字的颜色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Calibration_Cross(uint16_t x, uint16_t y, uint16_t color)
{
    uint16_t i;

    for(i=x-10;i<x+10;i++)
    {
        LCD_Point(i,y,color);
    }
    for(i=y-10;i<y+10;i++)
    {
        LCD_Point(x,i,color);
    }
}



/**********************************************************
* @funcName ：LCD与触摸屏建立联系，屏幕校准
* @brief    ：LCD_Touch_Calibration
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Touch_Calibration(void)
{
    uint8_t i;
    TOUCH_XY touch_xy[4];
    /* {20,20} {220, 20} {20,300} {220,300} */
    TOUCH_XY lcd_xy[4] = {\
                          {20,20},\
                          {lcddev.width-20,20},\
                          {20, lcddev.height-20},\
                          {lcddev.width-20,lcddev.height-20}\
                         };/* 用于校准的LCD区域点坐标*/
    double temp1,temp2;

THIS:  
    /***************获取校准区域坐标***************/
    /* 通过特定的LCD坐标区域获取该区域的触摸屏坐标 */
    /* 以下坐标是竖屏时数据 */
    /* LCD第0号校准区域坐标点(20,20) */
    /* LCD第1号校准区域坐标点(220,20) */
    /* LCD第2号校准区域坐标点(20,300) */
    /* LCD第3号校准区域坐标点(220,300) */
    for(i=0;i<4;i++)
    {
        LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,RED);      /* 画第i个区域十字 */
        while(T_PEN);                                            /* 等待触摸屏按下 */
        delay_ms(10);                                            /* 延时10ms消除按下抖动 */
        touch_xy[i] = Touch_Smoothing();                         /* 获取触摸屏坐标 */
        while(!T_PEN);                                           /* 等待抬起 */
        LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,WHITE);    /* 擦除区域十字 */
        delay_ms(150);                                           /* 延时100ms，消除抬起抖动 */
    }

    /****************判断点的有效性****************/
    /* 求点0到点1的距离平方temp1 */
    temp1 = (touch_xy[0].x - touch_xy[1].x)*(touch_xy[0].x - touch_xy[1].x) + (touch_xy[0].y - touch_xy[1].y)*(touch_xy[0].y - touch_xy[1].y);
    /* 求点2到点3的距离平方temp2 */
    temp2 = (touch_xy[2].x - touch_xy[3].x)*(touch_xy[2].x - touch_xy[3].x) + (touch_xy[2].y - touch_xy[3].y)*(touch_xy[2].y - touch_xy[3].y);
    /* 判断是否在误差范围内 */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* 重新取点坐标 */
        goto THIS;
    }

    /* 求点0到点2的距离平方temp1 */
    temp1 = (touch_xy[0].x - touch_xy[2].x)*(touch_xy[0].x - touch_xy[2].x) + (touch_xy[0].y - touch_xy[2].y)*(touch_xy[0].y - touch_xy[2].y);
    /* 求点1到点3的距离平方temp2 */
    temp2 = (touch_xy[1].x - touch_xy[3].x)*(touch_xy[1].x - touch_xy[3].x) + (touch_xy[1].y - touch_xy[3].y)*(touch_xy[1].y - touch_xy[3].y);
    /* 判断是否在误差范围内 */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* 重新取点坐标 */
        goto THIS;
    }

    /* 求点0到点3的距离平方temp1 */
    temp1 = (touch_xy[0].x - touch_xy[3].x)*(touch_xy[0].x - touch_xy[3].x) + (touch_xy[0].y - touch_xy[3].y)*(touch_xy[0].y - touch_xy[3].y);
    /* 求点1到点2的距离平方temp2 */
    temp2 = (touch_xy[1].x - touch_xy[2].x)*(touch_xy[1].x - touch_xy[2].x) + (touch_xy[1].y - touch_xy[2].y)*(touch_xy[1].y - touch_xy[2].y);
    /* 判断是否在误差范围内 */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* 重新取点坐标 */
        goto THIS;
    }

    /***************开始计算LCD和触摸屏的关系量并接收***************/
    /***
    * lcd和触摸屏的xy坐标的偏移量
    * lcd和触摸屏的xy坐标的比例系数
    * 取两点区域坐标(LCD与触摸屏)求比例系数(取两次，求平均值，减小误差)
    * 注意:只针对对角线的点
    * 公式: xfac = (LCD_x1 - LED_x2) / (触x1 - 触x2)
    *       yfac = (LCD_y1 - LED_y2) / (触y1 - 触y2)
    * 备注: xfac、yfac：比例系数
    *      LCD_x1、LCD_y1：LCD第一个坐标点的坐标
    *      LCD_x2、LCD_y2：LCD第二个坐标点的坐标
    *      触x1、触y1：触摸屏第一个坐标点的坐标
    *      触x2、触y2：触摸屏第二个坐标点的坐标
    */
    tp_dev.xfac = ((float)(lcddev.width-20) - 20.0) / (touch_xy[3].x - touch_xy[0].x);
    tp_dev.xfac += (20.0 - (float)(lcddev.width-20)) / (touch_xy[2].x - touch_xy[1].x);
    tp_dev.xfac /= 2;
    tp_dev.yfac = ((float)(lcddev.height-20) - 20.0) / (touch_xy[3].y - touch_xy[0].y);
    tp_dev.yfac += ((float)(lcddev.height-20) - 20.0) / (touch_xy[2].y - touch_xy[1].y);
    tp_dev.yfac /= 2;

    /***
    * 获取lcd和触摸屏的xy坐标的偏移量
    * 已经求出比例系数，只需取一个区域坐标点(LCD和触摸屏)就可以求偏移量(取两次，求平均值，减小误差)
    * 公式: xc = LCD_x - xfac * 触x
    *       yc = LCD_y - yfac * 触y
    * 备注: b_x、d_y：x、y的偏移量
    *       xc、yc：比例系数
    *       LCD_x、LCD_y：LCD一个校准区域坐标点的坐标
    *       触x、触y：触摸屏一个校准区域坐标点的坐标
    */
    tp_dev.xc = 20 - tp_dev.xfac * touch_xy[0].x;
    tp_dev.xc += (lcddev.width-20) - tp_dev.xfac * touch_xy[3].x;
    tp_dev.xc /= 2;
    tp_dev.yc = 20 - tp_dev.yfac * touch_xy[0].y;
    tp_dev.yc += (lcddev.height-20) - tp_dev.yfac * touch_xy[3].y;
    tp_dev.yc /= 2;
}



/**********************************************************
* @funcName ：读取电阻触摸屏触摸物理坐标
* @brief    ：Touch_Read_XY
* @param    ：OUCH_XY* coord (触摸物理坐标)
* @retval   ：uint8_t --- (1有效触摸 0无效触摸)
* @details  ：
* @fn       ：
************************************************************/
uint8_t Touch_Read_XY(TOUCH_XY* coord)
{
    TOUCH_XY touch_xy1;
    TOUCH_XY touch_xy2;

    //获取触摸屏坐标
    touch_xy1 = Touch_Smoothing();
    touch_xy2 = Touch_Smoothing();

    /* 前后两次采样在+-TP_ERR_RANGE内 */
    if (((touch_xy2.x <= touch_xy1.x && touch_xy1.x < touch_xy2.x + TP_ERR_RANGE)  || \
         (touch_xy1.x <= touch_xy2.x && touch_xy2.x < touch_xy1.x + TP_ERR_RANGE)) && \
        ((touch_xy2.y <= touch_xy1.y && touch_xy1.y < touch_xy2.y + TP_ERR_RANGE)  || \
         (touch_xy1.y <= touch_xy2.y && touch_xy2.y < touch_xy1.y + TP_ERR_RANGE)))
    {
        coord->x = (touch_xy1.x + touch_xy2.x) / 2;
        coord->y = (touch_xy1.y + touch_xy2.y) / 2;
        
        return 1;
    }

    return 0;
}




/**********************************************************
* @funcName ：读取电阻触摸屏触摸坐标(物理坐标或屏幕坐标)
* @brief    ：Touch_Scan
* @param    ：uint8_t mode
*   @arg    ：0 --- 读取物理坐标   1 --- 读取屏幕坐标
* @retval   ：TOUCH_XY --- (坐标)
* @details  ：
* @fn       ：
************************************************************/
TOUCH_XY Touch_Scan(uint8_t mode)
{
    TOUCH_XY coord;
    coord.x=0;
    coord.y=0;
    
    if(T_PEN == 0)
    {
        if(mode == 0)                /* 电阻屏物理坐标 */
        {
            Touch_Read_XY(&coord);
        }
        else                         /* 屏幕坐标 */
        {
            if(Touch_Read_XY(&coord))
            {
                coord.x = (uint16_t)(tp_dev.xfac * coord.x + tp_dev.xc);
                coord.y = (uint16_t)(tp_dev.yfac * coord.y + tp_dev.yc);
            }
        }
    }
    
    return coord;
}



/**********************************************************
* @funcName ：触摸屏初始化
* @brief    ：Touch_Init
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void Touch_Init(void)
{
    tp_dev.touchtype = 0;                   /* 默认设置(电阻屏 & 竖屏) */
    tp_dev.touchtype |= lcddev.dir & 0X01;  /* 根据LCD判定是横屏还是竖屏 */
    
    Touch_IO_Init();                        /* 触摸屏相关引脚配置 */
    
    if(tp_dev.touchtype & 0X01)             /* 如果是横屏 */
    {
        tp_dev.comX = TOUCH_Y;
        tp_dev.comY = TOUCH_X;
    }
    else
    {
        tp_dev.comX = TOUCH_X;
        tp_dev.comY = TOUCH_Y;
    }
}


