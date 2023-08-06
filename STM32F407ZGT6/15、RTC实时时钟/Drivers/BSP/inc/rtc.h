#ifndef _RTC_H
#define _RTC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /****************     结构体定义     ****************/
    /* RTC结构体 */
    typedef struct
    {
        uint16_t year;  /* 年 */
        uint8_t month;  /* 月 */
        uint8_t day;    /* 日 */
        uint8_t week;   /* 星期 1-7 星期一到星期五 */
        uint8_t hour;   /* 时 */
        uint8_t min;    /* 分 */
        uint8_t sec;    /* 秒 */
        uint8_t ampm;   /* AM/PM, 0=AM/24H; 1=PM/12H; */
    } RTC_DATES;


    /****************    变量外部声明   *****************/
    extern RTC_DATES rtc;


    /****************    函数外部声明   *****************/
    uint16_t RTC_Read_Bkr(uint8_t bkrx);                          /* RTC读取后备区域SRAM */
    void RTC_Write_Bkr(uint8_t bkrx, uint16_t data);              /* RTC写入后备区域SRAM */
    static uint8_t RTC_Init_Mode(void);                           /* RTC进入初始化模式 */
    static uint8_t RTC_Wait_Synchro(void);                        /* 等待RSF同步 */
    static uint8_t RTC_DecDcd(uint8_t val);                       /* 十进制转换为BCD码 */
    static uint8_t RTC_DcdDec(uint8_t val);                       /* BCD码转换为十进制数据 */
    uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day);  /* 通过年月日获取星期数据 */

    uint8_t RTC_Init(void);                                       /* 对RTC进行初始化设置 */
    uint8_t RTC_Set_Time(RTC_DATES rtc);                          /* 设置RTC时间 */
    void RTC_Get_Time(void);                                      /* 获取RTC时间 */
    void RTC_Set_Alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);    /* 设置RTC闹钟 */
    void RTC_Set_WakeUp(uint8_t wksel, uint16_t cnt);                            /* 设置RTC周期中断 */

#endif
