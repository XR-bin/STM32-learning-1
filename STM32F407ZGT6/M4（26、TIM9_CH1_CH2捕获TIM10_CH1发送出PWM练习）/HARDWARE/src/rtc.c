#include "stm32f4xx.h" 
#include "rtc.h"
#include "stdio.h"

/*****************************************
*函数功能  ：BCD数据转十进制数据
*函数名    ：BCD_DEC_Transition 
*函数参数  ：u8 bcd
*函数返回值：u8
*描述      ：
*******************************************/
u8 BCD_DEC_Transition(u8 bcd)
{
  return  (bcd >> 4) * 10 + (bcd & 0x0f);
}

/*****************************************
*函数功能  ：十进制数据转BCD数据
*函数名    ：DEC_BCD_Transition
*函数参数  ：u8 dec
*函数返回值：u8
*描述      ：
*******************************************/
u8 DEC_BCD_Transition(u8 dec)
{
  return  (dec / 10) << 4 | (dec % 10);
}

/************************************
*函数功能  ：对RTC进行初始化设置
*函数名    ：RTC_Init
*函数参数  ：RTC_DATES rtc
*函数返回值：void
*描述      ：
*************************************/
void RTC_Init(RTC_DATES rtc)
{
  /*电源控制器*/
  //使能电源控制器时钟
  RCC->APB1ENR |= 1<<28;
  //电源控制寄存器
  PWR->CR |= 1<<8;  //使能对RTC的写访问
  
  if(RTC->BKP0R != 0xff)  //用于避免复位后重新设时间值(只对复位有用，断电后该位会清零，依然会重新设时间值)
  {
    /*时钟源选择*/
    //备份域控制寄存器
    RCC->BDCR &= ~(3<<8);
    RCC->BDCR |= (1<<8);             //选择外部低速时钟作为时钟源
    RCC->BDCR |= 1<<0;               //开启外部低速震荡器(LSE)
    while(!(RCC->BDCR & (1<<1)));    //外部低速震荡器就绪
    RCC->BDCR |= (1<<15);            //RTC时钟使能
    RCC->BDCR &= ~(1<<16);
    
    /*RTC配置*/
    //解除RTC内部的访问保护
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    //RTC控制寄存器
    RTC->CR &= ~(1 << 6);   //选择格式 24
    RTC->CR &= ~(1 << 5);   //使用影子寄存器 
    //恢复写保护
    RTC->WPR = 0xFF;
    /*时间设置*/
    //设置时间
    RTC_Set_Time(rtc.hour,rtc.min,rtc.sec,rtc.pm);
    //设置年月日星期
    RTC_Set_date(rtc.year,rtc.month,rtc.day,rtc.week);
    //避免复位后重新设时间值，但断电重启该位会自动清零。
    RTC->BKP0R = 0xff;   
  }
}

/*****************************************
*函数功能  ：设置RTC的时、分、秒、星期
*函数名    ：RTC_Set_Time
*函数参数  ：u8 hour,u8 min,u8 sec,u8 pm
*函数返回值：void
*描述      ：
*******************************************/
void RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 pm)
{
  u32 data;
  
  //解除RTC内部的访问保护
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
  
  //进入初始化状态
  RTC->ISR |= 1<<7;             //进入初始化
  while(!(RTC->ISR & (1<<6)));  //等待进入完成
  //开始设置时间
  data = (u32)(DEC_BCD_Transition(pm)<<22)  |
         (u32)(DEC_BCD_Transition(hour)<<16)|
         (u32)(DEC_BCD_Transition(min)<<8)  |
         (u32)(DEC_BCD_Transition(sec));
  RTC->TR = data;
  //退出初始化模式
  RTC->ISR &= ~(1<<7);
  //恢复写保护
  RTC->WPR = 0xFF;
}

/**********************************************
*函数功能  ：设置RTC的年、星期、月、日
*函数名    ：RTC_Set_date
*函数参数  ：u16 year,u8 week,u8 month,u8 day
*函数返回值：void
*描述      ：
************************************************/
void RTC_Set_date(u8 year,u8 month,u8 day,u8 week)
{
    u32 data;
  
  //解除RTC内部的访问保护
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
  
  //进入初始化状态
  RTC->ISR |= 1<<7;             //进入初始化
  while(!(RTC->ISR & (1<<6)));  //等待进入完成
  //开始设置时间
  data = (u32)(DEC_BCD_Transition(year)<<16)|
         (u32)(DEC_BCD_Transition(week)<<13)|
         (u32)(DEC_BCD_Transition(month)<<8)|
         (u32)(DEC_BCD_Transition(day));
  RTC->DR = data;
  //退出初始化模式
  RTC->ISR &= ~(1<<7);
  //恢复写保护
  RTC->WPR = 0xFF;
}

/************************************
*函数功能  ：获取RTC的时、分、秒
*函数名    ：RTC_Read_Time
*函数参数  ：RTC_DATES *rtc
*函数返回值：void
*描述      ：
*************************************/
void RTC_Read_Time(RTC_DATES *rtc)
{
  u32 data;
  
  //解除RTC内部的访问保护
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
  
  RTC->ISR &=~(1 << 5);  //不同步  清零
  //等待日历影子寄存器同步
  while(!(RTC->ISR & (1<<5)));
  //获取获取时间数据
  data = RTC->TR;
  //时间数据转换为十进制数据
  rtc->hour = BCD_DEC_Transition((data>>16)&0x3f); 
  rtc->min = BCD_DEC_Transition((data>>8)&0x7f);
  rtc->sec = BCD_DEC_Transition(data&0x7f);
  //恢复写保护
  RTC->WPR = 0xFF;
}

/************************************
*函数功能  ：获取RTC的时、分、秒
*函数名    ：RTC_Read_Time
*函数参数  ：RTC_DATES *rtc
*函数返回值：void
*描述      ：
*************************************/
void RTC_Read_date(RTC_DATES *rtc)
{
  u32 data;
  
  //解除RTC内部的访问保护
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
  
  RTC->ISR &=~(1 << 5);  //不同步  清零
  //等待日历影子寄存器同步
  while(!(RTC->ISR & (1<<5)));
  //获取获取时间数据
  data = RTC->DR;
  //时间数据转换为十进制数据
  rtc->year  = BCD_DEC_Transition((data>>16)&0xff); 
  rtc->week  = BCD_DEC_Transition((data>>13)&0x7);
  rtc->month = BCD_DEC_Transition((data>>8)&0x1f);
  rtc->day   = BCD_DEC_Transition(data&0x3f);
  //恢复写保护
  RTC->WPR = 0xFF;
}














