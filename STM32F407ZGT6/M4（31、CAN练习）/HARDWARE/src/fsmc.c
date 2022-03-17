#include "stm32f4xx.h"   
#include "lcd.h"
#include "fsmc.h"

/*************************** FSMC 驱 动 LCD ******************************/

/*****************************************************
*函数功能  ：批量复用引脚函数
*函数名    ：MyGPIO_AF_Set
*函数参数  ：void
*函数返回值：void
*描述      ：该函数可以批量复用同一类GPIO的引脚
********************************************************/
void MyGPIO_AF_Set(GPIO_TypeDef* GPIOx,u32 BITx,u8 AFx)
{  
	u8 pos;
	u8 i;
	for(i=0;i<16;i++)
	{
		pos = i;
		if((1<<pos)&BITx)  //判断该引脚是否要复用
		{
      //pos>>3是用于判断是高位引脚还是地位引脚
			GPIOx->AFR[pos>>3]&=~(0X0F<<((pos&0X07)*4));
			GPIOx->AFR[pos>>3]|=(u32)AFx<<((pos&0X07)*4);
		}
	}
}

/****************************************************************************
*函数功能  ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
*函数名    ：FSMC_LCD_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*     数据线：//复用 推挽   速度100M
*           DO0---PD14	DO1---PD15	DO2---PD0	DO3---PD1
*           DO4---PE7	DO5---PE8	DO6---PE9	DO7---PE10
*           DO8---PE11	DO9---PE12	DO10---PE13	DO11---PE14
*           DO12---PE15	DO13---PD8	DO14---PD9	DO15---PD10
*     控制线：//复用  推挽      速度100M
*           WR---PD5	RD---PD4	CS---PG12 	
*           RS/DC(数据/命名)---PF12	 
*           BL（背光）--PB15   
*     整理：
*          PB：15
*          PD：0-1  4-5  8-10  14-15  
*          PE：7-15
*          PF：12
*          PG：12           
*     需要设置的是FSMC的Bank1的区域4
***************************************************************************/
void FSMC_LCD_Init(void)
{
  /********* 配置GPIO *********/
  //GPIOx时钟使能
  RCC->AHB1ENR |= ((1<<1)|(0xf<<3));
  
  //PB口
  //背光灯，配置通用输出
  //端口模式寄存器
  GPIOB->MODER &= ~(0xc0000000);
  GPIOB->MODER |= (0x40000000);
  //端口输出类型寄存器
  GPIOB->OTYPER &= ~(1<<15);
  //端口输出速度寄存器
  GPIOB->OSPEEDR |= (0x0c000000);
  
  //PD口
  //配置复用输出
  //端口模式寄存器
  GPIOD->MODER &= ~(0xf03f0f0f);
  GPIOD->MODER |= (0xa02a0a0a);
  //端口输出类型寄存器
  GPIOD->OTYPER &= ~(0xc733);
  //端口输出速度寄存器
  GPIOD->OSPEEDR |= (0xf03f0f0f);
  //端口复用寄存器    批量
  MyGPIO_AF_Set(GPIOD,0xc733,12);
  
  //PE口
  //配置复用输出
  //端口模式寄存器
  GPIOE->MODER &= ~(0xffffc000);
  GPIOE->MODER |= (0xaaaa8000);
  //端口输出类型寄存器
  GPIOE->OTYPER &= ~(0xff80);
  //端口输出速度寄存器
  GPIOE->OSPEEDR |= (0xffffc000);
  //端口复用寄存器    批量
  MyGPIO_AF_Set(GPIOE,0xff80,12);
  
  //PF口
  //配置复用输出
  //端口模式寄存器
  GPIOF->MODER &= ~(3<<2*12);
  GPIOF->MODER |= (2<<2*12);
  //端口输出类型寄存器
  GPIOF ->OTYPER &= ~(1<<12);
  //端口输出速度寄存器
  GPIOF->OSPEEDR |= (3<<2*12);
  //端口复用寄存器
  GPIOF->AFR[1] &= ~(0xf<<(4*(12-8)));
  GPIOF->AFR[1] |= (12<<(4*(12-8)));

  
  //PG口
  //端口模式寄存器
  GPIOG->MODER &= ~(3<<2*12);
  GPIOG->MODER |= (2<<2*12);
  //端口输出类型寄存器
  GPIOG->OTYPER &= ~(1<<12);
  //端口输出速度寄存器
  GPIOG->OSPEEDR |= (3<<2*12);
  //端口复用寄存器
  GPIOG->AFR[1] &= ~(0xf<<(4*(12-8)));
  GPIOG->AFR[1] |= (12<<(4*(12-8)));

  /********** 关背光 **********/
  LCD_BL_L; 
  
  /********* 配置FSMC *********/
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

  //使能FSMC时钟 
  RCC->AHB3ENR|=1<<0;    

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
  FSMC_Bank1->BTCR[7] |= 2<<0;     //地址设置阶段的持续时间  2×HCLK 时钟周期
  //SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
  FSMC_Bank1E->BWTR[6] |= 0<<28; 	//模式A
  FSMC_Bank1E->BWTR[6] |= 2<<8; 	//数据阶段的持续时间为6ns*2个HCLK=12ns
  FSMC_Bank1E->BWTR[6] |= 0<<0;		//地址设置阶段的持续时间为1个HCLK =6ns	
  
  //使能BANK1,区域4
  FSMC_Bank1->BTCR[6]|=1<<0;
}




/***************************FSMC 驱 动 外部SRAN******************************/
/************************************************************************
*函数功能  ：对特定的GPIO口进行FSMC复用，并设置FSMC，用于驱动外部SRAN
*函数名    ：FSMC_SRAM_Init
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*     需要配置的引脚： 
*                   PD：0/1/4/5/8/9/10/11/12/13/14/15 
*                   PE：0/1/7/8/9/10/11/12/13/14/15
*                   PF：0/1/2/3/4/5/12/13/14/15
*                   PG：0/1/2/3/4/5/10
*              需要设置的是FSMC的Bank1的区域3
*************************************************************************/
void FSMC_SRAM_Init(void)
{
  /********* 配置GPIO *********/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (0xf<<3);
  
  //特别注意！！！！！
  //PG10,AF12(CS放到最前面,防止复位后CS非法变低,破坏原有数据)
  GPIOG->AFR[1] &= ~(0xf<<(4*(10-8)));
  GPIOG->AFR[1] |= (12<<(4*(10-8)));
  
  //PD口
  //配置复用输出
  //端口模式寄存器
  GPIOD->MODER &= ~(0xffff0f0f);
  GPIOD->MODER |= (0xaaaa0a0a);
  //端口输出类型寄存器
  GPIOD->OTYPER &= ~(0xff33);
  //端口输出速度寄存器
  GPIOD->OSPEEDR |= (0xffff0f0f);
  //端口复用寄存器    批量
  MyGPIO_AF_Set(GPIOD,0xff33,12);
  
  //PE口
  //配置复用输出
  //端口模式寄存器
  GPIOE->MODER &= ~(0xffffc00f);
  GPIOE->MODER |= (0xaaaa800a);
  //端口输出类型寄存器
  GPIOE->OTYPER &= ~(0xff83);
  //端口输出速度寄存器
  GPIOE->OSPEEDR |= (0xffffc00f);
  //端口复用寄存器    批量
  MyGPIO_AF_Set(GPIOE,0xff83,12);
  
  //PF口
  //配置复用输出
  //端口模式寄存器
  GPIOF->MODER &= ~(0xff000fff);
  GPIOF->MODER |= (0xaa000aaa);
  //端口输出类型寄存器
  GPIOF ->OTYPER &= ~(0xf03f);
  //端口输出速度寄存器
  GPIOF->OSPEEDR |= (0xff000fff);
  //端口复用寄存器
  MyGPIO_AF_Set(GPIOF,0xf03f,12);

  
  //PG口
  //端口模式寄存器
  GPIOG->MODER &= ~(0x00300fff);
  GPIOG->MODER |= (0x00200aaa);
  //端口输出类型寄存器
  GPIOG->OTYPER &= ~(0x043f);
  //端口输出速度寄存器
  GPIOG->OSPEEDR |= (0x00300fff);
  //端口复用寄存器
  MyGPIO_AF_Set(GPIOG,0x003f,12);
  
    /********* 配置FSMC *********/
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

  //使能FSMC时钟 
  RCC->AHB3ENR|=1<<0;    

  /*LED用的是Bank1的区域3*/
  //这里我们使用NE3 ，也就对应BTCR[4],[5]。				    
	FSMC_Bank1->BTCR[4]=0X00000000;
	FSMC_Bank1->BTCR[5]=0X00000000;
	FSMC_Bank1E->BWTR[4]=0x0FFFFFFF;
  //SRAM/NOR-Flash 片选控制寄存器
  FSMC_Bank1->BTCR[4] &= ~(1<<14);   //读写使用统一的时序 
  FSMC_Bank1->BTCR[4] |= 1<<12;      //在存储区域内使能了写入操作
  FSMC_Bank1->BTCR[4] &= ~(3<<4);
  FSMC_Bank1->BTCR[4] |= (1<<4);    //定义外部存储器器件宽度为16
  //SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）
  FSMC_Bank1->BTCR[5] &= ~(3<<28);   //访问模式 A
  FSMC_Bank1->BTCR[5] |= 5<<8;       //数据阶段的持续时间  2×HCLK时钟周期
  FSMC_Bank1->BTCR[5] |= 2<<0;       //地址设置阶段的持续时间  2×HCLK时钟周期
  //SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器)
  FSMC_Bank1E->BWTR[4] &= ~(3<<28);    //模式A
  FSMC_Bank1E->BWTR[4] &= ~(0xf<<20);  //CLK 信号的时钟分频比保留不设置 
  FSMC_Bank1E->BWTR[4] |= (0x3<<16);   //总线周转阶段的持续时间3个HCLK时钟周期
  FSMC_Bank1E->BWTR[4] |= (0x3<<8); 	 //数据阶段的持续时间为6ns*3个HCLK=18ns
  FSMC_Bank1E->BWTR[4] &= ~(0xf<<4);   //地址保持阶段的持续时间保留不设置
  FSMC_Bank1E->BWTR[4] |= (0x2<<0);		 //地址设置阶段的持续时间为2个HCLK =12ns	
  
  //使能BANK1,区域3
  FSMC_Bank1->BTCR[4]|=1<<0;

}






