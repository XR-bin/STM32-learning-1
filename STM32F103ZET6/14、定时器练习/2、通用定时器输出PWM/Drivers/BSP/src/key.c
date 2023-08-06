#include "delay.h"
#include "key.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\key.c
*              .\Drivers\BSP\inc\key.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-06-16
* @brief     ：按键驱动配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：KEY_Init
* @brief    ：对KEY对应的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            KEY0     PE4
*            KEY1     PE3
*            KEY_UP   PA0
*                                 -------------输出模式
* @fn       ：
************************************************************/
void KEY_Init(void)
{
    /* 端口时钟使能 */
    RCC->APB2ENR |= (1<<6);
    RCC->APB2ENR |= (1<<2);

    /* 端口模式配置(输入) */
    /* 端口上下拉 */
    GPIOE->CRL &= ~((0xf<<4*3)|(0xf<<4*4));
    GPIOE->CRL |= ((0x8<<4*3)|(0x8<<4*4));
    GPIOE->ODR |= (3<<3);                  /* F1系列的 调为高电平就是才正式变为上拉模式 */

    GPIOA->CRL &= ~(0xf<<4*0);
    GPIOA->CRL |= (0x8<<4*0);
    GPIOA->ODR |= (0<<0);
}



/**********************************************************
* @funcName ：KEY_Scan
* @brief    ：按键扫描函数
* @param    ：void
* @retval   ：uint8_t   1--KEY0  2--KEY1  3--KEY_UP
* @details  ：
*            KEY0、KEY1 低电平按下，高电平抬起
*            KEY_UP     高电平按下，低电平抬起
* @fn       ：
************************************************************/
uint8_t KEY_Scan(void)
{
    uint8_t temp = 0;
    static uint8_t key_flay = 1;

    if(key_flay && ( KEY_UP==1 || KEY0==0 || KEY1==0 ))
    {
        delay_ms(10);
        if( KEY_UP==1 || KEY0==0 || KEY1==0 )
        {
            key_flay = 0;
            if(KEY0==0)        temp = 1;
            else if(KEY1==0)   temp = 2;
            else if(KEY_UP==1) temp = 3;
        }
    }

    if(KEY_UP==0 && KEY0==1 && KEY1==1)
    {
        key_flay = 1;
    }

    return temp;
}








