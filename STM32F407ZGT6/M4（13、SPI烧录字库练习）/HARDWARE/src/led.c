#include "stm32f4xx.h"   
#include "led.h"
#include "beep.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/*****************************************************
*函数功能  ：对LED对应的GPIO口进行初始化设置
*函数名    ：LED_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
             LED1     PF6
             LED2     PF9
             LED3     PF10
             LED4     PC0
             高电平灭，低电平亮-----输出模式
********************************************************/
void LED_Init(void)
{
  //CPIOx口时钟使能
  RCC->AHB1ENR |= (1<<5);
  RCC->AHB1ENR |= (1<<2);
  
  //端口模式寄存器
  GPIOF->MODER &= ~((3<<2*6)|(3<<2*9)|(3<<2*10));
  GPIOF->MODER |= (1<<2*6)|(1<<2*9)|(1<<2*10);
  
  GPIOC->MODER &= ~(3<<0);
  GPIOC->MODER |= (1<<0);
  
  //端口类型寄存器
  GPIOF->OTYPER &= ~((1<<6)|(1<<9)|(1<<10));
  
  GPIOC->OTYPER &= ~(1<<0);
  
  //端口输出速率寄存器
  GPIOF->OSPEEDR &= ~((3<<2*6)|(3<<2*9)|(3<<2*10));
  GPIOF->OSPEEDR |= ((2<<2*6)|(2<<2*6)|(2<<2*10));
  
  GPIOC->OSPEEDR &= ~(3<<0);
  GPIOC->OSPEEDR |= (2<<0);
  
  //端口输出数据寄存器
  GPIOF->ODR |= ((1<<6)|(1<<9)|(1<<10));
  GPIOC->ODR |= (1<<0);
}

/************************************************************
*函数功能  ：LED灯全亮
*函数名    ：LED_All_ON
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_All_ON(void)
{
  LED1_ON;
  LED2_ON;
  LED3_ON;
  LED4_ON;
}

/************************************************************
*函数功能  ：所有LED全灭
*函数名    ：LED_All_OFF
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_All_OFF(void)
{
  LED1_OFF;
  LED2_OFF;
  LED3_OFF;
  LED4_OFF;
}

/************************************************************
*函数功能  ：选择灯灭
*函数名    ：LED_All_ON
*函数参数  ：u8
*函数返回值：void
*描述      ：
***************************************************************/
void LED_X_OFF(u8 n)
{
  switch(n)
  {
    case 0: LED1_OFF;break;
    case 1: LED2_OFF;break;
    case 2: LED3_OFF;break;
    case 3: LED4_OFF;break;
  }
}

/************************************************************
*函数功能  ：选择灯亮
*函数名    ：LED_All_OFF
*函数参数  ：u8
*函数返回值：void
*描述      ：
***************************************************************/
void LED_X_ON(u8 n)
{
  switch(n)
  {
    case 0: LED1_ON;break;
    case 1: LED2_ON;break;
    case 2: LED3_ON;break;
    case 3: LED4_ON;break;
  }
}

/************************************************************
*函数功能  ：流水灯
*函数名    ：LED_Flash1
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash1(void)
{
  static int i = 0;
  LED_X_ON(i);
  delay_ms(300);
  LED_X_OFF(i);
  i++;
  if(i>3)
  {
    i=0;
  }
}

/************************************************************
*函数功能  ：流水灯+蜂鸣器
*函数名    ：LED_Flash2
*函数参数  ：void
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash2(int n)
{
  static int i = 0;
  static int cont = 0;
  static int c = 1;
  LED_X_ON(i);
  cont++;
  if(cont >= 1000000/2*n)
  {
    BEEP_OFF;
  }
  if(cont >= 1000000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    BEEP_ON;
    c=!c;
    cont = 0;
  }
}

/************************************************************
*函数功能  ：流水灯
*函数名    ：LED_Flash3
*函数参数  ：int n
*函数返回值：void
*描述      ：
***************************************************************/
void LED_Flash3(int n)
{
  static int i = 0;
  static int cont = 0;
  
  LED_X_ON(i);
  cont++;
  
  if(cont >= 30000*n)
  {
    LED_X_OFF(i);
    i++;
    if(i>3)
    {
      i=0;
    }
    cont = 0;
  }
}






