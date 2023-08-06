#include "wwdg.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\wwdg.c
*              .\Drivers\BSP\inc\wwdg.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：窗口看门狗练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* 保存WWDG计数器的设置值,默认为最大. */
uint8_t g_wwdg_cnt = 0x7f;


/**********************************************************
* @funcName ：WWDG_Init
* @brief    ：初始化窗口看门狗
* @param    ：uint8_t tr (计数器值)
* @param    ：uint8_t wr (窗口值)
* @param    ：uint8_t fprer (分频系数（WDGTB）,范围:0~7,表示2^WDGTB分频)
* @retval   ：void
* @details  ：
* @fn       ：
*           Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=36Mhz
*           溢出时间=(4096*2^fprer)*(tr-0X3F)/PCLK3
*           假设fprer=4,tr=7f,PCLK1=36Mhz
*           则溢出时间=4096*16*64/36Mhz=116.50ms
************************************************************/
void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    uint32_t pri;

    RCC->APB1ENR |= 1 << 11;        /* 使能wwdg时钟 */
    g_wwdg_cnt = tr & g_wwdg_cnt;   /* 初始化WWDG_CNT. */
    WWDG->CFR |= fprer << 7;        /* PCLK1/4096再除2^fprer */
    WWDG->CFR &= 0XFF80;
    WWDG->CFR |= wr;                /* 设定窗口值 */
    WWDG->CR |= g_wwdg_cnt;         /* 设定计数器值 */
    WWDG->CR |= 1 << 7;             /* 开启看门狗 */

    /* NVIC配置 */
    /* 优先级分组(在主函数里写) */
    /* 计算优先级编码值 */
    /* 5--101  2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    pri = NVIC_EncodePriority(5, 1, 1);     /* 该中断抢占优先为1，子优先级为1 */
    /* 将编码值写入具体中断源 */
    NVIC_SetPriority(WWDG_IRQn,pri);
    /* 使能NVIC响应通道 */
    NVIC_EnableIRQ(WWDG_IRQn); 

    WWDG->SR = 0X00;                /* 清除提前唤醒中断标志位 */
    WWDG->CFR |= 1 << 9;            /* 使能提前唤醒中断 */
}



/**********************************************************
* @funcName ：WWDG_Set_Counter
* @brief    ：重设置WWDG计数器的值
* @param    ：uint8_t cnt (计数值)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void WWDG_Set_Counter(uint8_t cnt)
{
    WWDG->CR = (cnt & 0x7F);   /* 重设置7位计数器 */
}



/**********************************************************
* @funcName ：WWDG_IRQHandler
* @brief    ：窗口看门狗中断服务程序
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void WWDG_IRQHandler(void)
{  
    if (WWDG->SR & 0X01)                /* 先判断是否发生了WWDG提前唤醒中断 */
    {
        WWDG->SR = 0X00;                /* 清除提前唤醒中断标志位 */
        WWDG_Set_Counter(g_wwdg_cnt);   /* 重设窗口看门狗的值! */
        GPIOF->ODR ^= (1<<10);          /* LED1绿灯闪烁 */
    }
}



