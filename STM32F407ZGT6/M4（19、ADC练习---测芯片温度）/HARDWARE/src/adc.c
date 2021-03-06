#include "stm32f4xx.h"   
#include "stdio.h"
#include "delay.h"
#include "adc.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

/******************************************
*函数名    ：ADC1_Init_In5
*函数功能  ：ADC1的第五个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA5----------ADC12_IN5   模拟模式
*********************************************/
void ADC1_Init_In5(void)
{
  /*GPIOx寄存器配置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  //端口模式寄存器(模拟模式)
  GPIOA->MODER &= ~(3<<(2*5));
  GPIOA->MODER |= (3<<(2*5));
  
  /*ADCx寄存器配置*/
  //ADCx时钟使能
  RCC->APB2ENR |= 1<<8;
  //ADCx控制寄存器1   CR1
  ADC1->CR1 &= ~(0xffffffff);
  //ADCx控制寄存器2   CR2
  ADC1->CR2 &= ~(0xffffffff);
  ADC1->CR2 |= 1<<10;
  //ADCx采样时间寄存
  ADC1->SMPR2 &= ~(7<<(3*5));
  ADC1->SMPR2 |= (7<<(3*5));
  //ADCx规则序列寄存器
  ADC1->SQR1 &= ~(0xf<<20);
  ADC1->SQR3 &= ~(0x1f<<0);
  ADC1->SQR3 |= (5<<0);
  //ADC通用控制寄存器
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  
  
  //ADCx使能
  ADC1->CR2 |= (1<<0);
}

/******************************************
*函数名    ：ADC1_In5_Data
*函数功能  ：获取ADC1通道5的转换数据
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换可调电阻数据
*********************************************/
u16 ADC1_In5_Data(void)
{
  u16 data;
  
  //开启规则通道转换开关
  ADC1->CR2 |= (1<<30);
  //等待转换完成
  while(!(ADC1->SR & (1<<1)));
  //读取转换后的数据
  data = ADC1->DR;
  
  return data;
}

/******************************************
*函数功能  ：ADC2的第4个通道初始化配置
*函数名    ：ADC2_Init_In4
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA4----------ADC12_IN4   模拟模式
*********************************************/
void ADC2_Init_In4(void)
{
  /*GPIOx寄存器配置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  //端口模式寄存器(模拟模式)
  GPIOA->MODER &= ~(3<<(2*4));
  GPIOA->MODER |= (3<<(2*4));
  
  /*ADCx寄存器配置*/
  //ADCx时钟使能
  RCC->APB2ENR |= (1<<9);
  //ADCx控制寄存器1   CR1
  ADC2->CR1 &= ~(0xffffffff);
  //ADCx控制寄存器2   CR2
  ADC2->CR2 &= ~(0xffffffff);
  ADC2->CR2 |= (1<<10);
  //ADCx采样时间寄存
  ADC2->SMPR2 &= ~(7<<(3*4));
  ADC2->SMPR2 |= (7<<(3*4));
  //ADCx规则序列寄存器
  ADC2->SQR1 &= ~(0xf);
  ADC2->SQR3 &= ~(0x1f);
  ADC2->SQR3 = (4<<0);
  //ADC通用控制寄存器
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  
  ADC1->CR2 |= 1<<10;
  
  //ADCx使能
  ADC2->CR2 |= (1<<0);
}

/******************************************
*函数功能  ：获取ADC2通道4的转换数据
*函数名    ：ADC2_In4_Data
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换光敏电阻数据
*********************************************/
u16 ADC2_In4_Data(void)
{
  u16 data;
  
  //开启ADC2通道转换
  ADC2->CR2 |= (1<<30);
  //等待数据转换完成
  while(!(ADC2->SR & (1<<1)));
  //去取转换完成的ADC数据
  data = ADC2->DR;
  
  return data;
}

/******************************************
*函数功能  ：ADC1的第十六个通道初始化配置
*函数名    ：ADC1_Init_In16
*函数参数  ：void
*函数返回值：void 
*函数描述  ：无需引脚，用于测芯片温度
*********************************************/
void ADC1_Init_In16(void)
{ 
  /*ADCx寄存器配置*/
  //ADCx时钟使能
  RCC->APB2ENR |= 1<<8;
  //ADCx控制寄存器1   CR1
  ADC1->CR1 &= ~(0xffffffff);
  //ADCx控制寄存器2   CR2
  ADC1->CR2 &= ~(0xffffffff);
  ADC1->CR2 |= 1<<10;
  //ADCx采样时间寄存
  ADC1->SMPR1 &= ~(7<<18);
  ADC1->SMPR1 |= (7<<18);
  //ADCx规则序列寄存器
  ADC1->SQR1 &= ~(0xf<<20);
  ADC1->SQR3 &= ~(0x1f<<0);
  ADC1->SQR3 |= (16<<0);
  //ADC通用控制寄存器
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  ADC->CCR |= 1<<23;
  
  //ADCx使能
  ADC1->CR2 |= (1<<0);
}

/******************************************
*函数功能  ：获取ADC1通道16的转换数据
*函数名    ：ADC1_In16_Data
*函数参数  ：void
*函数返回值：u16
*函数描述  ：检测芯片温度
*********************************************/
u16 ADC1_In16_Data(void)
{
  u16 data;
  
  //开启规则通道转换开关
  ADC1->CR2 |= (1<<30);
  //等待转换完成
  while(!(ADC1->SR & (1<<1)));
  //读取转换后的数据
  data = ADC1->DR;
  
  return data;
}

/******************************************
*函数功能  ：对ADC1通道16转换的数据进行处理
*函数名    ：ADC1_Chip_Temperature
*函数参数  ：void
*函数返回值：float
*函数描述  ：用于检测芯片温度
*********************************************/
float ADC1_Chip_Temperature(void)
{
  u16 data;
  double stm_t;

  data=Get_Adc_Average(16,5);	//读取通道16,20次取平均
	stm_t = (float)data*(3.3/4096);		//电压值
	stm_t = (stm_t-0.76)/0.0025+25; 	//转换为温度值 
  
  return stm_t;
}
					  
/******************************************
*函数功能  ：对ADC1通道16转换的数据进行滤波
*函数名    ：Get_Adc_Average
*函数参数  ：u8 ch,u8 times
*函数返回值：u16
*函数描述  ：
*********************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
    temp_val+=ADC1_In16_Data();
		delay_ms(5);
	}
	return temp_val/times;
}  






