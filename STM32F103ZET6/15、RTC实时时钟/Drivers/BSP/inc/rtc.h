#ifndef __RTC_H
#define __RTC_H

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
        uint8_t week;   /* 星期 */
        uint8_t hour;   /* 时 */
        uint8_t min;    /* 分 */
        uint8_t sec;    /* 秒 */
    } RTC_DATES;



    /****************    变量外部声明   *****************/
    extern RTC_DATES rtc;



    /****************    函数外部声明   *****************/
    uint16_t RTC_Read_Bkr(uint8_t bkrx);                          /* RTC读取后备区域SRAM */
    void RTC_Write_Bkr(uint8_t bkrx, uint16_t data);              /* RTC写入后备区域SRAM */
    static long RTC_DateSec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* 将年月日时分秒转换成秒钟数 */
    static uint8_t Leap_Year(uint16_t year);                      /* 判断闰年还是平年 */

    uint8_t RTC_Init(void);                                       /* 对RTC进行初始化设置 */
    uint8_t RTC_Set_Time(RTC_DATES rtc);                          /* 设置RTC时间 */
    void RTC_Get_Time(void);                                      /* 获取RTC时间 */
    uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day);  /* 通过年月日获取星期数据 */
    uint8_t RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);    /* 设置RTC闹钟 */

#endif
