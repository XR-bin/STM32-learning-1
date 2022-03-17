#include "stm32f4xx.h"     
#include "stdio.h"

/******************************************************************
*函数功能  ：对串口1对应的GPIO口进行初始化和对串口1的寄存器进行设置
*函数名    ：USART1_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Init(u32 baud)
{
  u8 pri;
  
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  
  //端口模式寄存器
  GPIOA->MODER &= ~((3<<2*9)|(3<<2*10));
  GPIOA->MODER |= ((2<<2*9)|(2<<2*10));
  
  //端口输出类型寄存器
  GPIOA->OTYPER &= ~(1<<9); 
  
  //端口输出速率寄存器
  GPIOA->OSPEEDR &= ~(3<<2*9);
  GPIOA->OSPEEDR |= (2<<2*9);
  
  //端口输入上拉/下拉寄存器
  GPIOA->PUPDR &= ~(3<<2*10);
  
  //端口复用功能寄存器
  GPIOA->AFR[1] &= ~((0xf<<4*(9-8))|(0xf<<4*(10-8)));
  GPIOA->AFR[1] |= (7<<4*(9-8)|(7<<4*(10-8)));
  
  /*串口配置部分*/
  //串口时钟使能
  RCC->APB2ENR |= (1<<4);
  
  //控制寄存器CR1
  USART1->CR1 &= ~((1<<15)|(1<<12));
  USART1->CR1 |= ((1<<3)|(1<<2));
  
  //使能中断形式（CR1）
  USART1->CR1 |= (1<<5);
  USART1->CR1 |= (1<<4);
  
  //控制寄存器CR2
  USART1->CR2 &= ~(3<<12);
  
  //波特率寄存器
  USART1->BRR = 84*1000000/baud;
  
  /*NVIC配置*/
  //中断分组（在主函数）
  //计算中断优先级编码中断
  pri = NVIC_EncodePriority (5, 1, 2);  //5号分组形式，抢占1级，响应2级
  //将编码写入中断源
  NVIC_SetPriority(USART1_IRQn,pri);
  //使能NVIC通道
  NVIC_EnableIRQ(USART1_IRQn); 
  
  
  //串口使能
  USART1->CR1 |= (1<<13);
}

/******************************************************************
*函数功能  ：串口1发送一个字节数据（8位）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Send_Byte(u8 data)
{
  while(!(USART1->SR & (1<<6)));
  USART1->DR = data;
}

/******************************************************************
*函数功能  ：串口1接收一个字节数据（8位）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8 
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
u8 USART1_Receive_Byte(void)
{
  u8 data;
  
  while(!(USART1->SR & (1<<5)));
  data = USART1->DR;
  
  return data;
}

/******************************************************************
*函数功能  ：串口1发送一个字符串
*函数名    ：USART1_Send_Str
*函数参数  ：u8 *str
*函数返回值：void
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Send_Str(u8 *str)
{
  while(*str != '\0')
  {
    USART1_Send_Byte(*str);
    str++;
  }
}

/******************************************************************
*函数功能  ：串口1接收一个字符串
*函数名    ：USART1_Receive_Str
*函数参数  ：u8 *str
*函数返回值：void 
*描述      ：
            PA9   TX    输出
            PA10  RX    输入
*******************************************************************/
void USART1_Receive_Str(u8 *str)
{
  u8 data;
  while(1)
  {
    data = USART1_Receive_Byte();
    if(data == '#')
    {
      break;
    }
    *str = data;
    str++;
  }
  *str = '\0'; 
}

/******************************************************************
*函数功能  ：对串口2对应的GPIO口进行初始化和对串口2的寄存器进行设置
*函数名    ：USART2_Init
*函数参数  ：u32 baud
*函数返回值：void
*描述      ：
            PA2   TX    输出
            PA3   RX    输入
*******************************************************************/
void USART2_Init(u32 baud)
{
  /*GPIOx配置部分*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  
  //端口模式寄存器
  GPIOA->MODER &= ~((3<<2*2)|(3<<2*3));
  GPIOA->MODER |= ((2<<2*2)|(2<<2*3));
  
  //端口输出类型寄存器
  GPIOA->OTYPER &= ~(1<<2); 
  
  //端口输出速率寄存器
  GPIOA->OSPEEDR &= ~(3<<2*2);
  GPIOA->OSPEEDR |= (2<<2*2);
  
  //端口输入上拉/下拉寄存器
  GPIOA->PUPDR &= ~(3<<2*3);
  
  //端口复用功能寄存器
  GPIOA->AFR[0] &= ~((0xf<<4*2)|(0xf<<4*3));
  GPIOA->AFR[0] |= ((7<<4*2)|(7<<4*3));
  
  /*串口配置部分*/
  //串口时钟使能
  RCC->APB1ENR |= (1<<17);
  
  //控制寄存器CR1
  USART2->CR1 &= ~((1<<15)|(1<<12));
  USART2->CR1 |= ((1<<3)|(1<<2));
  
  //控制寄存器CR2
  USART2->CR2 &= ~(3<<12);
  
  //波特率寄存器
  USART2->BRR = 42*1000000/baud;
  //串口使能
  USART2->CR1 |= (1<<13);
}

/******************************************************************
*函数功能  ：串口2发送一个字节数据（8位）
*函数名    ：USART1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*描述      ：
            PA2   TX    输出
            PA3  RX    输入
*******************************************************************/
void USART2_Send_Byte(u8 data)
{
  while(!(USART2->SR & (1<<6)));
  USART2->DR = data;
}

/******************************************************************
*函数功能  ：串口2接收一个字节数据（8位）
*函数名    ：USART1_Receive_Byte
*函数参数  ：void
*函数返回值：u8 
*描述      ：
            PA2   TX    输出
            PA3  RX    输入
*******************************************************************/
u8 USART2_Receive_Byte(void)
{
  u8 data;
  
  while(!(USART2->SR & (1<<5)));
  data = USART2->DR;
  
  return data;
}



















/***********************************************************************************/
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
/***********************************************************************************/






