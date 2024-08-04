#include "fsmc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\fsmc.c
*              .\Drivers\BSP\inc\fsmc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：FSMC配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：MyGPIO_AF_Set
* @brief    ：批量复用引脚函数
* @param    ：GPIO_TypeDef* GPIOx (引脚组)
* @param    ：uint32_t BITx       (需要服用的引脚)
* @param    ：uint8_t AFx         (引脚服用)
* @retval   ：void
* @details  ：
* @fn       ：
*            该函数可以批量复用同一类GPIO的引脚
************************************************************/
void MyGPIO_AF_Set(GPIO_TypeDef* GPIOx, uint32_t BITx, uint8_t AFx)
{
    uint8_t pos;
    uint8_t i;
    for(i=0;i<16;i++)
    {
        pos = i;
        if((1<<pos)&BITx)  /* 判断该引脚是否要复用 */
        {
            /* pos>>3是用于判断是高位引脚还是地位引脚 */
            GPIOx->AFR[pos>>3]&=~(0X0F<<((pos&0X07)*4));
            GPIOx->AFR[pos>>3]|=(uint32_t)AFx<<((pos&0X07)*4);
        }
    }
}



/**********************************************************
* @funcName ：FSMC_LCD_Init
* @brief    ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线：
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     控制线： 通用输出   推挽   速度50M
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(数据/命名)---PF12
*           BL（背光）--PB15
*     整理：
*         PB：15
*         PD：0-1   4-5   8-10   14-15
*         PE：7-15
*         PF：12
*         PG：12
*
*     需要设置的是FSMC的Bank1的区域4
* @fn       ：
************************************************************/
void FSMC_LCD_Init(void)
{
    /* GPIOx时钟使能 */
    RCC->AHB1ENR |= ((1<<1)|(0xf<<3));
    /* 使能FSMC时钟 */
    RCC->AHB3ENR|=1<<0;

    /* PB口   PB：15 */
    /* 端口模式寄存器 */
    GPIOB->MODER &= ~(0xc0000000);
    GPIOB->MODER |= (0x40000000);      /* 输出 */
    /* 端口输出类型寄存器 */
    GPIOB->OTYPER &= ~(1<<15);         /* 推挽 */
    /* 端口输出速度寄存器 */
    GPIOB->OSPEEDR |= (0xc0000000);

    /* PD口   PD：0-1   4-5   8-10   14-15 */
    /* 配置复用输出 */
    /* 端口模式寄存器 */
    GPIOD->MODER &= ~(0xf03f0f0f);
    GPIOD->MODER |= (0xa02a0a0a);      /* 复用输出 */
    /* 端口输出类型寄存器 */
    GPIOD->OTYPER &= ~(0xc733);        /* 推挽 */
    /* 端口输出速度寄存器 */
    GPIOD->OSPEEDR |= (0xf03f0f0f);
    /* 端口复用寄存器    批量 */
    MyGPIO_AF_Set(GPIOD,0xc733,12);

    /* PE口   PE：7-15 */
    /* 配置复用输出 */
    /* 端口模式寄存器 */
    GPIOE->MODER &= ~(0xffffc000);
    GPIOE->MODER |= (0xaaaa8000);      /* 复用输出 */
    /* 端口输出类型寄存器 */
    GPIOE->OTYPER &= ~(0xff80);        /* 推挽 */
    /* 端口输出速度寄存器 */
    GPIOE->OSPEEDR |= (0xffffc000);
    /* 端口复用寄存器    批量 */
    MyGPIO_AF_Set(GPIOE,0xff80,12);

    /* PF口   PF：12 */
    /* 配置复用输出 */
    /* 端口模式寄存器 */
    GPIOF->MODER &= ~(3<<2*12);
    GPIOF->MODER |= (2<<2*12);         /* 复用输出 */
    /* 端口输出类型寄存器 */
    GPIOF ->OTYPER &= ~(1<<12);        /* 推挽 */
    /* 端口输出速度寄存器 */
    GPIOF->OSPEEDR |= (3<<2*12);
    /* 端口复用寄存器 */
    GPIOF->AFR[1] &= ~(0xf<<(4*(12-8)));
    GPIOF->AFR[1] |= (12<<(4*(12-8)));

    /* PG口   PG：12 */
    /* 配置复用输出 */
    /* 端口模式寄存器 */
    GPIOG->MODER &= ~(3<<2*12);
    GPIOG->MODER |= (2<<2*12);         /* 复用输出 */
    /* 端口输出类型寄存器 */
    GPIOG->OTYPER &= ~(1<<12);         /* 推挽 */
    /* 端口输出速度寄存器 */
    GPIOG->OSPEEDR |= (3<<2*12);
    /* 端口复用寄存器 */
    GPIOG->AFR[1] &= ~(0xf<<(4*(12-8)));
    GPIOG->AFR[1] |= (12<<(4*(12-8)));

    /********** 关背光 **********/
    GPIOB->ODR &= ~(1<<15);

    /***************FSMC寄存器配置***************/
    /***
    * 注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
    *
    *      FSMC_BCRx 和 FSMC_BTRx，组合成 BTCR[8]寄存器组，他们的对应关系如下：
    *      BTCR[0]对应 FSMC_BCR1，BTCR[1]对应 FSMC_BTR1
    *      BTCR[2]对应 FSMC_BCR2，BTCR[3]对应 FSMC_BTR2
    *      BTCR[4]对应 FSMC_BCR3，BTCR[5]对应 FSMC_BTR3
    *      BTCR[6]对应 FSMC_BCR4，BTCR[7]对应 FSMC_BTR4
    *
    *      FSMC_BWTRx 则组合成 BWTR[7]，他们的对应关系如下：
    *      BWTR[0]对应 FSMC_BWTR1，BWTR[2]对应 FSMC_BWTR2，
    *      BWTR[4]对应 FSMC_BWTR3，BWTR[6]对应 FSMC_BWTR4，
    *      BWTR[1]、BWTR[3]和 BWTR[5]保留，没有用到。
    */
    /* LED用的是Bank1的区域4 */
    /* 这里我们使用NE4 ，也就对应BTCR[6],[7] */
    /* 清零操作 */
    FSMC_Bank1->BTCR[6]=0X00000000;
    FSMC_Bank1->BTCR[7]=0X00000000;
    FSMC_Bank1E->BWTR[6]=0X00000000;
    /* SRAM/NOR-Flash 片选控制寄存器 */
    FSMC_Bank1->BTCR[6] |= 1<<14;       /* 读写使用不同的时序 */
    FSMC_Bank1->BTCR[6] |= 1<<12;       /* 在存储区域内使能了写入操作 */
    FSMC_Bank1->BTCR[6] |= 1<<4;        /* 定义外部存储器器件宽度为16 */
    FSMC_Bank1->BTCR[6] &= ~(1<<1);     /* 地址/数据线不复用 */
    /* SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）*/
    FSMC_Bank1->BTCR[7] &= ~(3<<28);    /* 访问模式 A */
    FSMC_Bank1->BTCR[7] |= 0X3C<<8;     /* 数据阶段的持续时间60×HCLK时钟周期 */
    FSMC_Bank1->BTCR[7] &= ~(0xF<<4);   /* 地址保持阶段的持续时间1×HCLK 时钟周期 */
    FSMC_Bank1->BTCR[7] |= (0xF<<0);    /* 地址设置阶段的持续时间15×HCLK 时钟周期 */
    /* SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器) */
    FSMC_Bank1E->BWTR[6] &= ~(3<<28);   /* 模式A */
    FSMC_Bank1E->BWTR[6] |= 0x8<<8;     /* 数据阶段的持续时间8×HCLK时钟周期 */
    FSMC_Bank1E->BWTR[6] &= ~(0xF<<4);  /* 地址保持阶段的持续时间1×HCLK 时钟周期 */
    FSMC_Bank1E->BWTR[6] |= (0x9<<0);   /* 地址设置阶段的持续时间9×HCLK 时钟周期 */

    /* 使能BANK1,区域4 */
    FSMC_Bank1->BTCR[6]|=1<<0;
}




