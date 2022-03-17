#include "sys.h"
#include "stdio.h" 

/*****************************************************
*函数功能  ：对USART1对应的GPIO口进行初始化设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART1_Init(u32 baud)
{
  u8 pri;
  /*GPIOx初始化设置*/
  //GPIOx时钟使能
  RCC->APB2ENR |= (1<<2);
  
  //端口配置寄存器
  GPIOA->CRH &= ~((0xf<<4*(9-8))|(0xf<<4*(10-8)));
  GPIOA->CRH |= ((11<<4*(9-8))|(4<<4*(10-8)));
  
  //复用重映射寄存器
  AFIO->MAPR &= ~(1<<2);
  
  //外设复位寄存器
  RCC->APB2RSTR |= (1<<14);
  RCC->APB2RSTR &= ~(1<<14);
  
  /*串口寄存器初始化设置*/
  //串口时钟使能
  RCC->APB2ENR |= (1<<14);
  
  //控制寄存器CR1
  USART1->CR1 &= ~(1<<12);
  USART1->CR1 |= ((1<<2)|(1<<3));
  
  //串口中断触发形式（CR1设置）
  USART1->CR1 |= (1<<5);
  
  //控制寄存器CR2
  USART1->CR2 &= ~(3<<12);
  
  //波特率寄存器
  USART1->BRR = 72*1000000 / baud;
  
  /*NVIC配置*/
  //优先级分组(在主函数里写)
  //计算优先级编码值
  pri = NVIC_EncodePriority (5, 1, 2);  //第五分组形式，抢占1级，响应2级
  //将编码值写入具体中断源
  NVIC_SetPriority(USART1_IRQn,pri);
  //使能NVIC响应通道
  NVIC_EnableIRQ(USART1_IRQn); 
  
  //串口使能
  USART1->CR1 |= (1<<13);
}

/*****************************************************
*函数功能  ：串口1发送一个字节的数据（u8）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART1_Send_Byte(u8 data)
{
  while(!(USART1->SR & (1<<6)));
  USART1->DR = data;
}

/*****************************************************
*函数功能  ：串口1发送一个字符串
*函数名    ：USART1_Send_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART1_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    while(!(USART1->SR & (1<<6)));
    USART1->DR = *str;
    str++;
  }
}

/*****************************************************
*函数功能  ：串口1接收一个字节的数据（u8）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
  
  while(!(USART1->SR & (1<<5)));
  data = USART1->DR;
  
  return data;
}

/*****************************************************
*函数功能  ：串口1接收一个字符串
*函数名    ：USART1_Receive_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART1_Receive_Str(u8 *str)
{
  while(1)
  {
    while(!(USART1->SR & (1<<5)));
    *str = USART1->DR;
    if(*str == '#')
    {
      break;
    }
    str++;
  }
  *str = '\0';
}

/*****************************************************
*函数功能  ：对USART1对应的GPIO口进行初始化设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
            PA2     TX     ---------输出
            PA3     RX     ---------输入
********************************************************/
void USART2_Init(u32 baud)
{
  /*GPIOx初始化设置*/
  //GPIOx时钟使能
  RCC->APB2ENR |= (1<<2);
  
  //端口配置寄存器
  GPIOA->CRL &= ~((0xf<<4*2)|(0xf<<4*3));
  GPIOA->CRL |= ((11<<4*2)|(4<<4*3));
  
  //复用重映射寄存器
  AFIO->MAPR &= ~(1<<3);
  
  //外设复位寄存器
  RCC->APB1RSTR |= (1<<17);
  RCC->APB1RSTR &= ~(1<<17);
  
  /*串口寄存器初始化设置*/
  //串口时钟使能
  RCC->APB1ENR |= (1<<17);
  
  //控制寄存器CR1
  USART2->CR1 &= ~(1<<12);
  USART2->CR1 |= ((1<<2)|(1<<3));
  
  //控制寄存器CR2
  USART2->CR2 &= ~(3<<12);
  
  //波特率寄存器
  USART2->BRR = 36*1000000 / baud;
  
  //串口使能
  USART2->CR1 |= (1<<13);
}

/*****************************************************
*函数功能  ：串口1发送一个字节的数据（u8）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
void USART2_Send_Byte(u8 data)
{
  while(!(USART2->SR & (1<<6)));
  USART2->DR = data;
}

/*****************************************************
*函数功能  ：串口1接收一个字节的数据（u8）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8
*描述      ：
            PA9     TX     ---------输出
            PA10    RX     ---------输入
********************************************************/
u8 USART2_Receive_Byte(void)
{
  u8 data;
  
  while(!(USART2->SR & (1<<5)));
  data = USART2->DR;
  
  return data;
}














//printf配置
/*****************************************************************************************/
#if 1
	#pragma import(__use_no_semihosting)             
	//标准库需要的支持函数                 
	struct __FILE 
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}; 
	/* FILE is typedef’ d in stdio.h. */ 
	FILE __stdout;       
	//定义_sys_exit()以避免使用半主机模式    
	 _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{      
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = (u8) ch;      
		return ch;
	}
	#endif 
/*****************************************************************************************/






