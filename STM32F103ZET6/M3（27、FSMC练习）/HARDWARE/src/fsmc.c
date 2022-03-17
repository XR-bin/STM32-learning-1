#include "sys.h"
#include "lcd.h"
#include "fsmc.h"

/*************************** FSMC 驱 动 LCD ******************************/

/****************************************************************************
*函数功能  ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
*函数名    ：FSMC_LCD_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*     数据线：//通用输出 推挽      速度50M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     控制线：//通用输出  推挽      速度50M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(数据/命名)---PG0	 
*           BL（背光）--PB0   
*     整理：
*         PB：0
*         PD：0-1  4-5  8-10  14-15  
*         PE：7-15
*         PG：0  12   
*     需要设置的是FSMC的Bank1的区域4
***************************************************************************/
void FSMC_LCD_Init(void)
{
  /*使能GPIOx时钟和FSMC时钟*/
  //GPIOB、D、E、G
  RCC->APB2ENR |= ((1<<3)|(1<<5)|(1<<6)|(1<<8));
  //FSMC
  RCC->AHBENR |= 1<<8;
  
  /***************GPIOx寄存器配置***************/
  //PB
  //背光灯，配置通用输出
  //端口配置寄存器
  GPIOB->CRL &= ~(0xf);
  GPIOB->CRL |= (0x3);
  
  //PD 
  //配置复用输出
  //端口配置寄存器
  GPIOD->CRL &= ~(0x00ff00ff);
  GPIOD->CRH &= ~(0xff000fff);
  GPIOD->CRL |= (0x00bb00bb);
  GPIOD->CRH |= (0xbb000bbb);
  
  //PE
  //配置复用输出
  //端口配置寄存器
  GPIOE->CRL &= ~(0xf0000000);
  GPIOE->CRH &= ~(0xffffffff);
  GPIOE->CRL |= (0xb0000000);
  GPIOE->CRH |= (0xbbbbbbbb);
  
  //PG
  //配置复用输出
  //端口配置寄存器
  GPIOG->CRL &= ~(0xf);
  GPIOG->CRH &= ~(0x000f0000);
  GPIOG->CRL |= (0xb);
  GPIOG->CRH |= (0x000b0000);
  
  /********** 关背光 **********/
  LCD_BL_L; 
  
  /***************FSMC寄存器配置***************/
  
/***********************************************************************
*注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
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
*************************************************************************/
  /*LED用的是Bank1的区域4*/
  //这里我们使用NE4 ，也就对应BTCR[6],[7]。
  //清零操作
  FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
  //SRAM/NOR-Flash 片选控制寄存器
  FSMC_Bank1->BTCR[6] |= 1<<14;   //读写使用不同的时序 
  FSMC_Bank1->BTCR[6] |= 1<<12;   //在存储区域内使能了写入操作
  FSMC_Bank1->BTCR[6] &= ~(3<<4);
  FSMC_Bank1->BTCR[6] |= (1<<4);  //定义外部存储器器件宽度为16
  //SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）
  FSMC_Bank1->BTCR[7] &= ~(3<<28); //访问模式 A
  FSMC_Bank1->BTCR[7] |= 0XF<<8;   //数据阶段的持续时间16×HCLK时钟周期
  FSMC_Bank1->BTCR[7] |= 1<<0;     //地址设置阶段的持续时间2×HCLK 时钟周期
  //SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
  FSMC_Bank1E->BWTR[6] |= 0<<28; 	//模式A
  FSMC_Bank1E->BWTR[6] |= 2<<8; 	//数据阶段的持续时间为6ns*2个HCLK=12ns
  FSMC_Bank1E->BWTR[6] |= 0<<0;		//地址设置阶段的持续时间为1个HCLK =6ns
  
  //使能BANK1,区域4
  FSMC_Bank1->BTCR[6]|=1<<0;
}








