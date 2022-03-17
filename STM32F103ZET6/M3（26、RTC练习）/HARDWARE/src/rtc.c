#include "sys.h"
#include "rtc.h"
#include "stdio.h"
#include "delay.h"

/**************************************************
*函数功能  ：对RTC进行初始化设置
*函数名    ：RTC_Init
*函数参数  ：RTC_DATES rtc
*函数返回值：void
*描述      ：
****************************************************/
void RTC_Init(RTC_DATES rtc)
{
  /*电源控制器*/
  //使能电源控制器时钟
  RCC->APB1ENR |= 1<<28;
  //使能备份时钟
  RCC->APB1ENR |= 1<<27;
  //电源控制寄存器
  PWR->CR |= 1<<8;           //允许写入RTC和后备寄存器
  if(BKP->DR1 != 0Xfe)
  {
    /*时钟源选择*/
    //备份域控制寄存器
    RCC->BDCR &= ~(3<<8);
    RCC->BDCR |= 1<<8;              //LSE振荡器作为RTC时钟
    RCC->BDCR |= 1<<0;              //外部32kHz振荡器开启
    while(!(RCC->BDCR & (1<<1)));   //外部32kHz振荡器就绪
    RCC->BDCR |= 1<<15;             //RTC时钟使能
    RCC->BDCR &= ~(1<<16);
    
    //等待确保RTC_CNT、 RTC_ALR或RTC_PRL已经被同步已经完成
    while(!(RTC->CRL & (1<<3)));
    
    /*RTC配置*/
    RTC->CRL |= 1<<4;    //进入配置模式
    RTC->PRLH=0;
    RTC->PRLL=0x7FFF;    //ST官方提供的一个参考值，可获得周期为1秒钟的信号
    //设置时间
    RTC_Set(rtc);
    //退出配置模式
    RTC->CRL&=~(1<<4);  
    //等待对RTC寄存器的写操作已经完成。
    while(!(RTC->CRL & (1<<5)));
    //备份区里写下标志，防止二次初始化
    BKP->DR1 = 0Xfe;
  }
}


/*****************************************************************
*函数功能  ：判断闰年还是平年
*函数名    ：Leap_Year
*函数参数  ：u16
*函数返回值：u8 
*描述      ：返回 1  闰年     返回 0  平年
*******************************************************************/
u8 Leap_Year(u16 year)
{
  //闰年：能被400整除。或者能被4整除但不能被100整除。
  if((year % 400 == 0)||((year % 4 == 0)&&(year % 100 != 0)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//平年月份天数数组
u8 common_year[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

/*****************************************************************
*函数功能  ：设置RTC的年、月、日、时、分、秒
*函数名    ：RTC_Set
*函数参数  ：RTC_DATES rtc
*函数返回值：u8 
*描述      ：返回 1  表示配置失败     返回 0   表示配置成功
*******************************************************************/
u8 RTC_Set(RTC_DATES rtc)
{
  u16 t;
	u32 sec_count = 0;    //用来记录秒数的
  //判断输入的年份是否有超出范围
  if((rtc.year<1970)||(rtc.year>2099)) return 1;
  //计算1970年到rtc.year一共经历了多少秒
  for(t=1970;t<rtc.year;t++)
  {
    if(Leap_Year(t))sec_count += 31622400;  //闰年一年有366 * 24 * 60 * 60 = 31622400 秒
    else sec_count += 31536000;             //平年一年有365 * 24 * 60 * 60 = 31536000 秒
  }
  //计算rtc.year里的1月到rtc.month一共经历了多少秒
  rtc.month -= 1;
  for(t=0;t<rtc.month;t++)
  {
    sec_count += common_year[t] * 86400;           //一天有24 * 60 * 60 = 86400秒
    if(Leap_Year(t) && (t==1)) sec_count += 86400; //闰年比平年的2月多1天
  }
  //计算rtc.month里的1月到rtc.day一共经历了多少秒
  sec_count += (rtc.day-1) * 86400;
  //计算rtc.day里的rtc.hour一共经历了多少秒
  sec_count += rtc.hour  * 3600;
  //计算rtc.day里的rtc.min一共经历了多少秒
  sec_count += rtc.min * 60;
  //计算rtc.day里的rtc.min一共经历了多少秒
  sec_count += rtc.sec;
  
  //进入配置模式
  RTC->CRL |= 1<<4;
  //正式设置时间
  RTC->CNTL=sec_count&0xffff;
	RTC->CNTH=sec_count>>16;
  //退出配置
  RTC->CRL &= ~(1<<4);
  //等待对RTC寄存器的写操作已经完成。
  while(!(RTC->CRL & (1<<5)));
  
  return 0;
}

/*****************************************************************
*函数功能  ：获取RTC的年、月、日、星期、时、分、秒
*函数名    ：RTC_Get
*函数参数  ：void
*函数返回值：RTC_DATES 
*描述      ：
*******************************************************************/
RTC_DATES RTC_Get(void)
{
  RTC_DATES rtc;
  u32 sec_count;   //用来接RTC->CNT寄存器的值
  u32 day;
  u32 temp;        
  
  //等待确保RTC_CNT、 RTC_ALR或RTC_PRL已经被同步已经完成。
  RTC->CRL &= ~(1<<3);
  while(!(RTC->CRL & (1<<3)));
  //获取时间(秒值)
  sec_count = RTC->CNTH;     //获取高16位的数据(秒数)
  sec_count <<= 16;
  sec_count += RTC->CNTL;    //获取低16位的数据(秒数)，整合后就是1970到现在经历的秒数
  
  //计算1970到现在的一共经历多少天
  day = sec_count / 86400;  //一天有24 * 60 * 60 = 86400秒
  //如果天数超出一天
  if(day != 0)
  {
    temp = 1970;     //起始年份
    //计算现在是哪一年(以1970为基准年份)
    while(day >= 365)
    {
      //判断是不是闰年
      if(Leap_Year(temp))
      {
        if(day>=366) day -= 366;
        else break;
      }
      else day -= 365;
      temp++;
    }
    //存储年
    rtc.year = temp;
    temp = 0;
    //计算现在是第几个月
    while(day>=28)
    {
      //判断是不是闰年且是不是二月
      if(Leap_Year(rtc.year) && (temp==1))
      {
        if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
      }
      else
      {
        //判断是不是大于这个月的天数
        if(day>common_year[temp]) day -= common_year[temp];
        else break;
      }
      temp++;
    }
    //存储月份和天数
    rtc.month = temp+1;
    rtc.day = day+1;
  }
  else
  {
    //如果RTC->CNT的值超不过86400(一天)，则赋下面的值
    rtc.year = 1970;
    rtc.month = 1;
    rtc.day = 1;
  }
  
  temp = sec_count % 86400;     		    //得到除去年月日后的秒数   	   
	rtc.hour = temp / 3600;     	        //小时
	rtc.min = (temp%3600) / 60; 	        //分钟	
	rtc.sec = (temp%3600) % 60; 	        //秒钟
  
  rtc.week = RTC_Week(rtc);             //获取周末时间
  
  return rtc;
}

/***********************************************************************
*函数功能  ：计算当前是第几个星期
*函数名    ：RTC_Week
*函数参数  ：RTC_DATES rtc
*函数返回值：u8
*描述      ：  
*    蔡勒(Zeller)公式：
*                 w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
*             W除以7，余数是几就是星期几。如果余数是0，则为星期日。
*      符号：
*           w：星期
*           c：世纪-1
*           y：年（年份后两位数）
*           m：月（m大于等于3，小于等于14，即在蔡勒公式中，某年的1、2月
*                 要看作上一年的13、14月来计算，比如2003年1月1日要看作
*                 2002年的13月1日来计算）
*           d：日
************************************************************************/
u8 RTC_Week(RTC_DATES rtc)
{
  u16 w;
  u8 c = rtc.year / 100;  //世纪-1
  u8 y = rtc.year % 100;  //年份后两位数
  u8 m = rtc.month;       //月
  u8 d = rtc.day;         //日
  
  //对月、年、世纪进行处理
  if(m <= 2)
  {
    //蔡勒公式中，某年的1、2月
    //要看作上一年的13、14月来计算
    //且如果刚好跨世纪，则世纪也得处理
    m += 12;
    y -= 1;
    if(rtc.year % 100 == 0) c -= 1;
  }
  
  //蔡勒公式:w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
  w = y + (u8)(y/4) + (u8)(c/4) - 2*c + (u8)(26*(m+1)/10) + d - 1;
  w %= 7;
  if(w != 0) return w;
  else return 7;
}







