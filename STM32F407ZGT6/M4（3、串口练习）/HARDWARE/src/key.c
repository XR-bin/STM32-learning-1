#include "stm32f4xx.h" 
#include "key.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}



/*****************************************************
*函数功能  ：对KEY对应的GPIO口进行初始化设置
*函数名    ：KEY_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
void KEY_Init(void)
{
  //端口时钟使能  GPIOA  GPIOE
  RCC->AHB1ENR |= ((1<<0)|(1<<4));
  //端口模式配置
  GPIOA->MODER &= ~(3<<0);
  GPIOA->MODER |= (0<<0);                         //通用输入
  
  GPIOE->MODER &= ~((3<<2*2)|(3<<3*2)|(3<<4*2));
  GPIOE->MODER |= ((0<<2*2)|(0<<3*2)|(0<<4*2));   //通用输入
  //端口上下拉
  GPIOA->PUPDR &= ~(0<<0);                        //无上下拉
  GPIOE->PUPDR &= ~((3<<2*2)|(3<<3*2)|(3<<4*2));  //无上下拉
}

/*****************************************************
*函数功能  ：按键扫描函数
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
u8 KEY_Scan(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  if(key_flay&&((KEY1&(1<<0))||(!(KEY2&(1<<2)))||(!(KEY2&(1<<3)))||(!(KEY2&(1<<4)))))
  {
    delay_ms(10);
    if((KEY1&(1<<0))||(!(KEY2&(1<<2)))||(!(KEY2&(1<<3)))||(!(KEY2&(1<<4))))
    {
      key_flay = 0;
      if(KEY1&(1<<0)) temp = 1;
      else if(!(KEY2&(1<<2))) temp = 2;
      else if(!(KEY2&(1<<3))) temp = 3;
      else if(!(KEY2&(1<<4))) temp = 4;
    }
  }
  
  if(!(KEY1&(1<<0))&&(KEY2&(1<<2))&&(KEY2&(1<<3))&&(KEY2&(1<<4)))
  {
    key_flay = 1;
  }
  
  return temp;
}


/*****************************************************
*函数功能  ：两个按键同时按
*函数名    ：KEY_Scan
*函数参数  ：void
*函数返回值：u8
*描述      ：
            KEY1  PA0    高电平按下
            KEY2  PE2    低电平按下
            KEY3  PE3    低电平按下
            KEY4  PE4    低电平按下
                   输入模式
********************************************************/
u8 KEY_Scan2(void)
{
  u8 temp = 0;
  static u8 key_flay = 1;
  static u32 cont = 0 ;
  if(key_flay&&((KEY1&(1<<0))||!(KEY2&(1<<2))))
  {
    cont++;
    if((KEY1&(1<<0))&&!(KEY2&(1<<2))&&(cont==250000))
    {
      delay_ms(10);
      if((KEY1&(1<<0))&&(!(KEY2&(1<<2))))
      {
        key_flay = 0;
        temp=12;
      }
    }
  }
  if( cont > 250000)
  {
    cont = 255000;
  }
  if(!(KEY1&(1<<0))&&(KEY2&(1<<2)))
  {
    key_flay = 1;
    cont = 0;
  }
  
  return temp;
}










