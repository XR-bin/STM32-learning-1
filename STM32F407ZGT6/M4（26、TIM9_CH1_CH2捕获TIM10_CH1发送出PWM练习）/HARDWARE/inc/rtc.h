#ifndef _RTC_H
#define _RTC_H

  #include "stm32f4xx.h" 
  
  //用于设置时间的结构体
  typedef struct
  {
    u8 year;
    u8 month;
    u8 day;
    u8 week;
    u8 hour;
    u8 min;
    u8 sec;
    u8 pm;
  }RTC_DATES;

  u8 BCD_DEC_Transition(u8 bcd);
  u8 DEC_BCD_Transition(u8 dec);
  void RTC_Init(RTC_DATES rtc);
  void RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 pm);
  void RTC_Set_date(u8 year,u8 month,u8 day,u8 week);
  void RTC_Read_Time(RTC_DATES *rtc);
  void RTC_Read_date(RTC_DATES *rtc);

#endif










