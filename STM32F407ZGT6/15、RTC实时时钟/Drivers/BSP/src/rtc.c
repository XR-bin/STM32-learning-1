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
    0,          /* 星期 1-7 星期一到星期五 */
    12,         /* 时 */
    0,          /* 分 */
    0,          /* 秒 */
    0           /* AM/PM, 0=AM/24H; 1=PM/12H; */
};



/**********************************************************
* @funcName ：RTC_Init
* @brief    ：对RTC进行初始化设置
* @param    ：void
* @retval   ：uint8_t  ---  (0配置成功  1/2配置失败)
* @details  ：
* @fn       ：
************************************************************/
uint8_t RTC_Init(void)
{
    uint16_t ssr = 0;       /* 存亚秒寄存器值 */
    uint16_t bkpflag = 0;   /* 备份值 判断是否之前设置过 */
    uint16_t retry = 200;   /* 等待上限 避免死循环 */
    uint32_t tempreg = 0;   /* 存放寄存器值 */

    rtc.week = RTC_Week(rtc.year, rtc.month, rtc.day);     /* 修正星期数据 */

    RCC->APB1ENR|=1<<28;        /* 使能电源接口时钟 */
    PWR->CR|=1<<8;              /* 后备区域访问使能(RTC+SRAM) */

    bkpflag = RTC_Read_Bkr(0);      /* 读取备份寄存器值 */

    if (bkpflag != 0X5050)          /* 之前使用的不是LSE */
    {
        RCC->CSR |= 1 << 0;            /* 内部低速振荡器总是使能 */
        while (!(RCC->CSR & 0x02));    /* 等待内部低速振荡器就绪 */

        RCC->BDCR |= 1 << 0;           /* 尝试开启外部低速震荡器 */
        while (retry && ((RCC->BDCR & 0X02) == 0))  /* 等待外部低速震荡器准备好 */
        {
            retry--;
            delay_ms(5);
        }

        tempreg = RCC->BDCR;        /* 读取BDCR的值 */
        tempreg &= ~(3 << 8);       /* 清零8/9位 */

        if (retry == 0)tempreg |= 1 << 9;   /* LSE开启失败,启动LSI. */
        else tempreg |= 1 << 8;             /* 选择LSE,作为RTC时钟 */

        tempreg |= 1 << 15;     /* 使能RTC时钟 */
        RCC->BDCR = tempreg;    /* 重新设置BDCR寄存器 */

        /* 关闭RTC寄存器写保护 */
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->CR = 0;

        if(RTC_Init_Mode())
        {
            RCC->BDCR = 1 << 16;/* 复位BDCR */
            delay_ms(10);
            RCC->BDCR = 0;      /*  结束复位 */
            return 2;           /* 进入RTC初始化模式失败 */
        }

        RTC->PRER = 0XFF;       /* RTC同步分频系数(0~7FFF),必须先设置同步分频,再设置异步分频,Frtc=Fclks/((Sprec+1)*(Asprec+1)) */
        RTC->PRER |= 0X7F << 16;/* RTC异步分频系数(1~0X7F) */
        RTC->CR &= ~(1 << 6);   /* RTC设置为,24小时格式 */
        RTC->ISR &= ~(1 << 7);  /* 退出RTC初始化模式 */
        RTC->WPR = 0xFF;        /* 使能RTC寄存器写保护 */

        if (bkpflag != 0X5051)  /* BKP0的内容既不是0X5050,也不是0X5051,说明是第一次配置,需要设置时间日期. */
        {
            RTC_Set_Time(rtc);  /* 设置时间 */
        }

        if (retry == 0)
        {
            RTC_Write_Bkr(0, 0X5051);   /* 标记已经初始化过了,使用LSI */
        }
        else 
        {
            RTC_Write_Bkr(0, 0X5050);   /* 标记已经初始化过了,使用LSE */
        }
    }
    else
    {
        retry = 10;     /* 连续10次SSR的值都没变化,则LSE死了. */
        ssr = RTC->SSR; /* 读取初始值 */

        while (retry)   /* 检测ssr寄存器的动态,来判断LSE是否正常 */
        {
            delay_ms(10);

            if (ssr == RTC->SSR)        /* 对比 */
            {
                retry--;
            }
            else 
            {
                break;
            }
        }

        if (retry == 0) /* LSE挂了,清除配置等待下次进入重新设置 */
        {
            RTC_Write_Bkr(0, 0XFFFF);   /* 标记错误的值 */
            RCC->BDCR = 1 << 16;        /* 复位BDCR */
            delay_ms(10);
            RCC->BDCR = 0;              /* 结束复位 */
        }
    }

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
    temp = RTC_BASE + 0x50 + bkrx * 4;
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
    temp = RTC_BASE + 0x50 + bkrx * 4;
    (*(uint16_t *)temp) = data;
}



/**********************************************************
* @funcName ：RTC_Init_Mode
* @brief    ：RTC进入初始化模式
* @param    ：void
* @retval   ：uint8_t  ---  (0,成功;1,失败)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t RTC_Init_Mode(void)
{
    uint32_t retry = 0XFFFFF;

    if (RTC->ISR & (1 << 6))return 0;

    RTC->ISR |= 1 << 7; /* 进入RTC初始化模式 */

    while (retry && ((RTC->ISR & (1 << 6)) == 0x00))
    {
        retry--;    /* 等待进入RTC初始化模式成功 */
    }
   
    if (retry == 0)
    {
        return 1;   /* 同步失败 */
    }
    else 
    {
        return 0;   /* 同步成功 */
    }
}



/**********************************************************
* @funcName ：RTC_DecDcd
* @brief    ：十进制转换为BCD码
* @param    ：uint8_t val (要转换的十进制数) 
* @retval   ：uint8_t  ---  (BCD码)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t RTC_DecDcd(uint8_t val)
{
    uint8_t bcdhigh = 0;

    while (val >= 10)
    {
        bcdhigh++;
        val -= 10;
    }

    return ((uint8_t)(bcdhigh << 4) | val);
}



/**********************************************************
* @funcName ：RTC_DcdDec
* @brief    ：BCD码转换为十进制数据
* @param    ：uint8_t val (要转换的BCD码) 
* @retval   ：uint8_t  ---  (十进制数据)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t RTC_DcdDec(uint8_t val)
{
    uint8_t temp = 0;
    temp = (val >> 4) * 10;
    return (temp + (val & 0X0F));
}



/**********************************************************
* @funcName ：RTC_Wait_Synchro
* @brief    ：等待RSF同步
* @param    ：void
* @retval   ：uint8_t  ---  (0同步成功  1同步失败)
* @details  ：
* @fn       ：
************************************************************/
static uint8_t RTC_Wait_Synchro(void)
{
    uint32_t retry = 0XFFFFF;
    /* 关闭RTC寄存器写保护 */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ISR &= ~(1 << 5);  /* 清除RSF位 */

    while (retry && ((RTC->ISR & (1 << 5)) == 0x00))
    {
        retry--;    /* 等待影子寄存器同步 */
    }
    
    if (retry == 0)return 1;/* 同步失败 */

    RTC->WPR = 0xFF;        /* 使能RTC寄存器写保护 */
    return 0;
}



/**********************************************************
* @funcName ：RTC_Set_Time
* @brief    ：设置RTC的年、月、日、时、分、秒、AM/PM
* @param    ：RTC_DATES *rtc (要设置的RTC时钟值)
* @retval   ：uint8_t  ---  (0配置成功  1配置失败)
* @details  ：
* @fn       ：
************************************************************/
uint8_t RTC_Set_Time(RTC_DATES rtc)
{
    uint32_t temp = 0;
    /* 关闭RTC寄存器写保护 */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    if (RTC_Init_Mode())return 1;   /* 进入RTC初始化模式失败 */

    temp = (((uint32_t)rtc.ampm & 0X01) << 22) | ((uint32_t)RTC_DecDcd(rtc.hour) << 16) | ((uint32_t)RTC_DecDcd(rtc.min) << 8) | (RTC_DecDcd(rtc.sec));
    RTC->TR = temp;
    temp = (((uint32_t)rtc.week & 0X07) << 13) | ((uint32_t)RTC_DecDcd(rtc.year) << 16) | ((uint32_t)RTC_DecDcd(rtc.month) << 8) | (RTC_DecDcd(rtc.day));
    RTC->DR = temp;
    RTC->ISR &= ~(1 << 7);  /* 退出RTC初始化模式 */
    return 0;
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
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;

    while (RTC_Wait_Synchro()); /* 等待同步 */

    temp1 = RTC->TR;
    temp2 = RTC->DR;
    rtc.hour = RTC_DcdDec((temp1 >> 16) & 0X3F);
    rtc.min = RTC_DcdDec((temp1 >> 8) & 0X7F);
    rtc.sec = RTC_DcdDec(temp1 & 0X7F);
    rtc.ampm = temp1 >> 22;

    rtc.year = RTC_DcdDec((temp2 >> 16) & 0XFF);
    rtc.month = RTC_DcdDec((temp2 >> 8) & 0X1F);
    rtc.day = RTC_DcdDec(temp2 & 0X3F);
    rtc.week = (temp2 >> 13) & 0X07;
}




/**********************************************************
* @funcName ：RTC_Week
* @brief    ：通过年月日获取星期数据
* @param    ：uint16_t year (年份)
* @param    ：uint8_t month (月份)
* @param    ：uint8_t day   (日期)
* @retval   ：uint8_t  ---  (1~7,代表周一~周日)
* @details  ：
* @fn       ：
************************************************************/
uint8_t RTC_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;
    /* 月修正数据表 */
    uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

    yearH = year / 100;
    yearL = year % 100;

    /*  如果为21世纪,年份数加100 */
    if (yearH > 19)yearL += 100;

    /*  所过闰年数只算1900年之后的 */
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];

    if (yearL % 4 == 0 && month < 3)temp2--;

    temp2 %= 7;

    if (temp2 == 0)temp2 = 7;

    return temp2;
}



/**********************************************************
* @funcName ：RTC_Set_Alarm
* @brief    ：设置RTC闹钟
* @param    ：uint8_t week   (星期)
* @param    ：uint8_t hour   (小时)
* @param    ：uint8_t min    (分钟)
* @param    ：uint8_t sec    (秒钟)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void RTC_Set_Alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t pri = 0;       /* 中断配置 */

    /* 关闭RTC寄存器写保护 */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &= ~(1 << 8);   /* 关闭闹钟A */

    while ((RTC->ISR & 0X01) == 0); /* 等待闹钟A修改允许 */

    RTC->ALRMAR = 0;        /* 清空原来设置 */
    RTC->ALRMAR |= 1 << 30; /* 按星期闹铃 */
    RTC->ALRMAR |= 0 << 22; /* 24小时制 */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(week) << 24;   /* 星期设置 */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(hour) << 16;   /* 小时设置 */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(min) << 8;     /* 分钟设置 */
    RTC->ALRMAR |= (uint32_t)RTC_DecDcd(sec);          /* 秒钟设置 */
    RTC->ALRMASSR = 0;      /* 不使用SUB SEC */
    RTC->CR |= 1 << 12;     /* 开启闹钟A中断 */
    RTC->CR |= 1 << 8;      /* 开启闹钟A */
    RTC->WPR = 0XFF;        /* 禁止修改RTC寄存器 */

    RTC->ISR &= ~(1 << 8);  /* 清除RTC闹钟A的标志 */
    EXTI->PR = 1 << 17;     /* 清除LINE17上的中断标志位 */
    EXTI->IMR |= 1 << 17;   /* 开启line17上的中断 */
    EXTI->RTSR |= 1 << 17;  /* line17上事件上升降沿触发 */

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(RTC_Alarm_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(RTC_Alarm_IRQn);
}



/**********************************************************
* @funcName ：RTC_Alarm_IRQHandler
* @brief    ：RTC闹钟中断
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void RTC_Alarm_IRQHandler(void)
{
    if (RTC->ISR & (1 << 8))    /* ALARM A中断? */
    {
        RTC->ISR &= ~(1 << 8);  /* 清除中断标志 */
        printf("ALARM A!\r\n");
    }

    EXTI->PR |= 1 << 17;        /* 清除中断线17的中断标志 */
}



/**********************************************************
* @funcName ：RTC_Set_WakeUp
* @brief    ：周期性唤醒定时器设置
* @param    ：wksel
*   @arg    ：000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;100,RTC
*   @arg    ：10x,ck_spre,1Hz;11x,1Hz,且cnt值增加2^16(即cnt+2^16)
*   @note   ：注意:RTC就是RTC的时钟频率,即RTCCLK!
* @param    ：cnt (自动重装载值.减到0,产生中断.)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void RTC_Set_WakeUp(uint8_t wksel, uint16_t cnt)
{
    uint32_t pri = 0;       /* 中断配置 */

    /* 关闭RTC寄存器写保护 */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->CR &= ~(1 << 10);  /* 关闭WAKE UP */

    while ((RTC->ISR & 0X04) == 0);	/* 等待WAKE UP修改允许 */

    RTC->CR &= ~(7 << 0);   /* 清除原来的设置 */
    RTC->CR |= wksel & 0X07;/* 设置新的值 */
    RTC->WUTR = cnt;        /* 设置WAKE UP自动重装载寄存器值 */
    RTC->ISR &= ~(1 << 10); /* 清除RTC WAKE UP的标志 */
    RTC->CR |= 1 << 14;     /* 开启WAKE UP 定时器中断 */
    RTC->CR |= 1 << 10;     /* 开启WAKE UP 定时器 */
    RTC->WPR = 0XFF;        /* 禁止修改RTC寄存器 */
    EXTI->PR = 1 << 19;     /* 清除LINE19上的中断标志位 */
    EXTI->IMR |= 1 << 19;   /* 开启line19上的中断 */
    EXTI->RTSR |= 1 << 19;  /* line19上事件上升降沿触发 */

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(RTC_WKUP_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(RTC_WKUP_IRQn);
}



/**********************************************************
* @funcName ：RTC_WKUP_IRQHandler
* @brief    ：RTC周期中断
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void RTC_WKUP_IRQHandler(void)
{ 
    if (RTC->ISR & (1 << 10))   /* WK_UP中断 */
    {
        RTC->ISR &= ~(1 << 10); /* 清除中断标志 */
        printf("实时时间:%d-%d-%d week:%d %d:%d:%d\r\n", rtc.year, rtc.month, rtc.day, rtc.week , rtc.hour, rtc.min, rtc.sec);
    }

    EXTI->PR |= 1 << 19;        /* 清除中断线19的中断标志 */
}









