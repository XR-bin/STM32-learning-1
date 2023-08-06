#include "rtc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\rtc.c
*              .\Drivers\BSP\inc\rtc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-01
* @brief     ：RTC配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* RTC时间 */
RTC_DATES rtc = {
    2023,       /* 年 */
    8,          /* 月 */
    1,          /* 日 */
    0,          /* 星期 */
    12,         /* 时 */
    0,          /* 分 */
    0           /* 秒 */
};



/**********************************************************
* @funcName ：RTC_Init
* @brief    ：对RTC进行初始化设置
* @param    ：void
* @retval   ：uint8_t  ---  (0配置成功  1配置失败)
* @details  ：
* @fn       ：
************************************************************/
uint8_t RTC_Init(void)
{
    uint32_t pri = 0;       /* 中断配置 */
    uint16_t bkpflag = 0;   /* 备份值 判断是否之前设置过 */
    uint16_t retry = 200;   /* 等待上限 避免死循环 */
    uint32_t tempreg = 0;   /* 存放寄存器值 */
    uint32_t clockfreq = 0; /* RTC所使用的时钟频率 */

    RCC->APB1ENR |= 1 << 28;        /* 使能电源时钟 */
    RCC->APB1ENR |= 1 << 27;        /* 使能备份时钟 */
    PWR->CR |= 1 << 8;              /* 取消备份区写保护 */

    bkpflag = RTC_Read_Bkr(0);      /* 读取备份寄存器值 */

    if (bkpflag != 0X5050)          /* 之前使用的不是LSE */
    {
        RCC->BDCR |= 1 << 0;        /* 开启外部低速振荡器 */

        while (retry && ((RCC->BDCR & 0X02) == 0))  /* 等待LSE准备好 */
        {
            retry--;
            delay_ms(5);
        }

        tempreg = RCC->BDCR;        /* 读取BDCR的值 */
        tempreg &= ~(3 << 8);       /* 清零8/9位 */

        if (retry == 0)             /* 开启LSE(外部时钟)失败，启动LSI(内部时钟) */
        {
            RCC->CSR |= 1 << 0;         /* LSI使能 */

            while (!(RCC->CSR & 0x02)); /* 等待LSI就绪 */

            tempreg |= 1 << 9;          /* LSE开启失败,启动LSI. */
            clockfreq = 40000 - 1;      /* LSI频率约40Khz(参考F103数据手册说明) */
            RTC_Write_Bkr(0, 0X5051);   /* 标记已经初始化过了,使用LSI */
        }
        else
        {
            tempreg |= 1 << 8;          /* 选择LSE,作为RTC时钟 */
            clockfreq = 32768 - 1;      /* LSE频率为32.769Khz */
            RTC_Write_Bkr(0, 0X5050);   /* 标记已经初始化过了,使用LSE */
        }

        tempreg |= 1 << 15;             /* 使能RTC时钟 */
        RCC->BDCR = tempreg;            /* 重新设置BDCR寄存器 */

        while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */
        while (!(RTC->CRL & (1 << 3))); /* 等待RTC寄存器同步 */

        RTC->CRH |= 1 << 0;             /* SECF = 1, 允许秒中断 */
        RTC->CRH |= 1 << 1;             /* ALRF = 1, 允许闹钟中断 */

        while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */

        RTC->CRL |= 1 << 4;             /* 允许配置 */
        RTC->PRLH = 0X0000;
        RTC->PRLL = clockfreq;          /* 时钟周期设置(有待观察,看是否跑慢了?)理论值：32767 */
        RTC->CRL &= ~(1 << 4);          /* 配置更新 */

        while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */

        if (bkpflag != 0X5051)          /* BKP0的内容既不是0X5050,也不是0X5051,说明是第一次配置,需要设置时间日期. */
        {
            RTC_Set_Time(rtc);   /* 设置时间 */
        }
    }
    else                /* 系统继续计时 */
    {
        retry = 30;     /* 避免卡死 */

        while ((!(RTC->CRL & (1 << 3)) && retry))   /* 等待RTC寄存器同步 */
        {
            delay_ms(5);
            retry--;
        }

        retry = 100;    /* 检测LSI/LSE是否正常工作 */

        tempreg = RTC->DIVL;            /* 读取DIVL寄存器的值 */

        while (retry)
        {
            delay_ms(5);
            retry--;

            if (tempreg != RTC->DIVL)   /* 对比DIVL和tempreg, 如果有差异, 则退出 */
            {
                break;                  /* DIVL != tempreg, 说明RTC在计数, 说明晶振没问题 */
            }
        }

        if (retry == 0)
        {
            RTC_Write_Bkr(0, 0XFFFF);   /* 标记错误的值 */
            RCC->BDCR = 1 << 16;        /* 复位BDCR */
            delay_ms(10);
            RCC->BDCR = 0;              /* 结束复位 */
            return 1;                   /* 初始化失败 */
        }
        else
        {
            RTC->CRH |= 0X01;           /* 允许秒中断 */
            while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */
        }
    }

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(RTC_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(RTC_IRQn);

    RTC_Get_Time();

    return 0;
}



/**********************************************************
* @funcName ：RTC_Read_Bkr
* @brief    ：RTC读取后备区域SRAM
* @param    ：uint8_t bkrx (后备区寄存器编号,范围:0~41)
* @retval   ：uint8_t  ---  (读取到的值)
* @details  ：
* @fn       ：
************************************************************/
uint16_t RTC_Read_Bkr(uint8_t bkrx)
{
    uint32_t temp = 0;
    temp = BKP_BASE + 4 + bkrx * 4;
    return (*(uint16_t *)temp); /* 返回读取到的值 */
}



/**********************************************************
* @funcName ：RTC_Write_Bkr
* @brief    ：RTC写入后备区域SRAM
* @param    ：uint8_t bkrx  (后备区寄存器编号,范围:0~41)
* @param    ：uint16_t data (要写入的数据,16位长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void RTC_Write_Bkr(uint8_t bkrx, uint16_t data)
{
    uint32_t temp = 0;
    PWR->CR |= 1 << 8;  /* 取消备份区写保护 */
    temp = BKP_BASE + 4 + bkrx * 4;
    (*(uint16_t *)temp) = data;
}



/**********************************************************
* @funcName ：RTC_Set_Time
* @brief    ：设置RTC的年、月、日、时、分、秒
* @param    ：RTC_DATES *rtc (要设置的RTC时钟值)
* @retval   ：uint8_t  ---  (0配置成功  1配置失败)
* @details  ：
* @fn       ：
*             以1970年1月1日为基准, 往后累加时间
*             合法年份范围为: 1970 ~ 2105年
************************************************************/
uint8_t RTC_Set_Time(RTC_DATES rtc)
{
    uint32_t seccount = 0;

    /* 判断输入的年份是否有超出范围 */
    if((rtc.year<1970)||(rtc.year>2099)) return 1;

    seccount = RTC_DateSec(rtc.year, rtc.month, rtc.day, rtc.hour, rtc.min, rtc.sec); /* 将年月日时分秒转换成总秒钟数 */

    /* 设置时钟 */
    RCC->APB1ENR |= 1 << 28;    /* 使能电源时钟 */
    RCC->APB1ENR |= 1 << 27;    /* 使能备份时钟 */
    PWR->CR |= 1 << 8;          /* 取消备份区写保护 */
    /* 上面三步是必须的! */

    RTC->CRL |= 1 << 4;         /* 允许配置 */
    RTC->CNTL = seccount & 0xffff;
    RTC->CNTH = seccount >> 16;
    RTC->CRL &= ~(1 << 4);      /* 配置更新 */

    while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */

    return 0;
}



/**********************************************************
* @funcName ：RTC_DateSec
* @brief    ：将年月日时分秒转换成秒钟数
* @param    ：uint16_t syear (年份)
* @param    ：uint8_t smon   (月份)
* @param    ：uint8_t sday   (日期)
* @param    ：uint8_t hour   (小时)
* @param    ：uint8_t min    (分钟)
* @param    ：uint8_t sec    (秒钟)
* @retval   ：long  ---  (转换后的秒钟数)
* @details  ：
* @fn       ：
*            以1970年1月1日为基准, 1970年1月1日, 0时0分0秒, 表示第0秒钟
*            最大表示到2105年, 因为uint32_t最大表示136年的秒钟数(不包括闰年)!
************************************************************/
static long RTC_DateSec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t Y, M, D, X, T;
    signed char monx = smon;/* 将月份转换成带符号的值, 方便后面运算 */

    if (0 >=  (monx -= 2))  /* 1..12 -> 11,12,1..10 */
    { 
        monx += 12;         /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    Y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400;  /* 公元元年1到现在的闰年数 */
    M = 367 * monx / 12 - 30 + 59;
    D = sday - 1;
    X = Y + M + D - 719162;                         /* 减去公元元年到1970年的天数 */
    T = ((X * 24 + hour) * 60 + min) * 60 + sec;    /* 总秒钟数 */
    return T;
}



/**********************************************************
* @funcName ：RTC_Get_Time
* @brief    ：获取RTC的年、月、日、星期、时、分、秒
* @param    ：void
* @retval   ：RTC_DATES  ---  (获取RTC时间)
* @details  ：
* @fn       ：
************************************************************/
void RTC_Get_Time(void)
{
    static uint16_t daycnt = 0; /*  */
    uint32_t seccount;   /* 用来接RTC->CNT寄存器的值 */
    uint32_t day;        /* 天数 */
    uint32_t temp;
    const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   /* 平年的月份日期表 */

    /* 获取时间(秒值) */
    seccount = RTC->CNTH;     /* 获取高16位的数据(秒数) */
    seccount <<= 16;
    seccount += RTC->CNTL;    /* 获取低16位的数据(秒数)，整合后就是1970到现在经历的秒数 */

    /* 计算1970到现在的一共经历多少天 */
    day = seccount / 86400;  /* 一天有24 * 60 * 60 = 86400秒 */

    /* 如果天数超出一天 */
    if(daycnt != day)
    {
        temp = 1970;     /* 起始年份 */
        daycnt = day;
        /* 计算现在是哪一年(以1970为基准年份) */
        while(day >= 365)
        {
            /* 判断是不是闰年 */
            if(Leap_Year(temp))
            {
                if(day>=366) day -= 366;
                else break;
            }
            else day -= 365;
            temp++;
        }

        rtc.year = temp;      /* 存储年 */

        temp = 0;

        /* 计算现在是第几个月 */
        while(day>=28)
        {
            /* 判断是不是闰年且是不是二月 */
            if(Leap_Year(rtc.year) && (temp==1))
            {
                if(temp>=29)temp-=29;//闰年的秒钟数
                else break; 
            }
            else
            {
                /* 判断是不是大于这个月的天数 */
                if(day>=month_table[temp]) day -= month_table[temp];
                else break;
            }
            temp++;
        }
        /* 存储月份和天数 */
        rtc.month = temp+1;       /* 得到月份 */
        rtc.day = day+1;          /* 得到日期 */
    }

    temp = seccount % 86400;         /* 得到除去年月日后的秒数 */
    rtc.hour = temp / 3600;          /* 小时 */
    rtc.min = (temp%3600) / 60;      /* 分钟 */
    rtc.sec = (temp%3600) % 60;      /* 秒钟 */

    rtc.week = RTC_Week(rtc.year, rtc.month, rtc.day);             /* 获取周末时间 */
}



/**********************************************************
* @funcName ：Leap_Year
* @brief    ：判断闰年还是平年
* @param    ：uint16_t year (年份)
* @retval   ：uint8_t  ---  (0-平年 1-闰年)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t Leap_Year(uint16_t year)
{
    /* 闰年：能被400整除。或者能被4整除但不能被100整除。 */
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/**********************************************************
* @funcName ：RTC_Week
* @brief    ：通过年月日获取星期数据
* @param    ：uint16_t year (年份)
* @param    ：uint8_t month (月份)
* @param    ：uint8_t day   (日期)
* @retval   ：uint8_t  ---  (0, 星期天; 1 ~ 6: 星期一 ~ 星期六)
* @details  ：
* @fn       ：
*            使用 基姆拉尔森计算公式 计算
************************************************************/
uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        year--;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}



/**********************************************************
* @funcName ：RTC_Set_Alarm
* @brief    ：设置RTC闹钟
* @param    ：uint16_t syear (年份)
* @param    ：uint8_t smon   (月份)
* @param    ：uint8_t sday   (日期)
* @param    ：uint8_t hour   (小时)
* @param    ：uint8_t min    (分钟)
* @param    ：uint8_t sec    (秒钟)
* @retval   ：uint8_t  ---  (0配置成功  1配置失败)
* @details  ：
* @fn       ：
************************************************************/
uint8_t RTC_Set_Alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = RTC_DateSec(syear, smon, sday, hour, min, sec); /* 将年月日时分秒转换成总秒钟数 */

    /* 设置时钟 */
    RCC->APB1ENR |= 1 << 28;    /* 使能电源时钟 */
    RCC->APB1ENR |= 1 << 27;    /* 使能备份时钟 */
    PWR->CR |= 1 << 8;          /* 取消备份区写保护 */
    /* 上面三步是必须的! */

    RTC->CRL |= 1 << 4;         /* 允许配置 */
    RTC->ALRL = seccount & 0xffff;
    RTC->ALRH = seccount >> 16;
    RTC->CRL &= ~(1 << 4);      /* 配置更新 */

    while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成 */

    return 0;
}



/**********************************************************
* @funcName ：RTC_IRQHandler
* @brief    ：RTC中断
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            根据RTC_CRL寄存器的 SECF 和 ALRF 位区分是哪个中断
************************************************************/
void RTC_IRQHandler(void)
{
    if (RTC->CRL & (1 << 0))    /* SECF = 1, 秒钟中断 */
    {
        RTC_Get_Time();   /* 更新时间 */
        RTC->CRL &= ~(1 << 0);  /* SECF = 0, 清秒钟中断 */
        printf("实时时间:%d-%d-%d week:%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.week , rtc.hour, rtc.min, rtc.sec);
    }

    /* 顺带处理闹钟标志 */
    if (RTC->CRL & (1 << 1))    /* ALRF = 1, 闹钟标志 */
    {
        RTC->CRL &= ~(1 << 1);  /* ALRF = 0, 清闹钟标志 */
        /* 输出闹铃时间 */
        printf("闹钟:%d-%d-%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.min, rtc.sec);
    }

    RTC->CRL &= ~(1 << 2);      /* OWF = 0, 清除溢出中断标志 */
    while (!(RTC->CRL & (1 << 5))); /* 等待RTC寄存器操作完成, 即等待RTOFF == 1 */
}


