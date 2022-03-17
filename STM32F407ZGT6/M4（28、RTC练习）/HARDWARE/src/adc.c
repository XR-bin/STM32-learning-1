#include "stm32f4xx.h"   
#include "stdio.h"
#include "adc.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

/***********************************************
*函数功能  ：ADC1的第五个通道初始化配置(无中断)
*函数名    ：ADC1_Init_In5
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA5----------ADC12_IN5   模拟模式
************************************************/
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

/***********************************************
*函数功能  ：ADC1的第五个通道初始化配置(有中断)
*函数名    ：ADC1_Init_In5
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA5----------ADC12_IN5   模拟模式
************************************************/
void ADC1_Init2_In5(void)
{
  u8 pri;
  
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
  ADC1->CR2 |= (1<<10);
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
  
  /*NVIC配置*/
  //中断分组（在主函数）
  //计算中断优先级编码中断
  pri = NVIC_EncodePriority (5, 1, 2);  //5号分组形式，抢占1级，响应2级
  //将编码写入中断源
  NVIC_SetPriority(ADC_IRQn,pri);
  //使能NVIC通道
  NVIC_EnableIRQ(ADC_IRQn); 
  
  //使能中断
  ADC1->CR1 |= (1<<5);
  //ADCx使能
  ADC1->CR2 |= (1<<0);
  
  //开启规则通道转换开关
  ADC1->CR2 |= (1<<30);
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
  ADC2->SQR1 &= ~(0xf<<20);
  ADC2->SQR3 &= ~(0x1f);
  ADC2->SQR3 = (4<<0);
  //ADC通用控制寄存器
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  
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
*函数功能  ：ADC1的第16个通道初始化配置
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
  ADC->CCR |= 1<<23;
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  
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
  u8 i;
  u16 data;
  float sum = 0;
  float stm_t[6];
  
  for(i=0;i<5;i++)
  {
    data = ADC1_In16_Data();
    stm_t[i] = (data - 943)/2.5+25;
    sum += stm_t[i];
  }
  
  stm_t[5] = sum/5;
  
  return stm_t[5];
}

/******************************************
*函数功能  ：ADC2的4、5通道初始化配置
*函数名    ：ADC2_Init_In4In5
*函数参数  ：void
*函数返回值：void 
*函数描述  ：无需引脚，用于测芯片温度
*********************************************/
void ADC2_Init_In4In5(void)
{
  /*GPIOx寄存器配置*/
  //GPIOx时钟使能
  RCC->AHB1ENR |= (1<<0);
  //端口模式寄存器(模拟模式)
  GPIOA->MODER &= ~((3<<(2*4))|(3<<(2*5)));
  GPIOA->MODER |= ((3<<(2*4))|(3<<(2*5)));

  /*ADCx寄存器配置*/
  //ADCx时钟使能
  RCC->APB2ENR |= (1<<9);
  //ADCx控制寄存器1   CR1
  ADC2->CR1 &= ~(0xffffffff);
  ADC2->CR1 |= 1<<8;
  //ADCx控制寄存器2   CR2
  ADC2->CR2 &= ~(0xffffffff);
  ADC2->CR2 |= (1<<10);
  //ADCx采样时间寄存
  ADC2->SMPR2 |= (7<<(3*4));
  ADC2->SMPR2 |= (7<<(3*5));
  //ADCx规则序列寄存器
  ADC2->SQR1 &= ~(0xf<<20);
  ADC2->SQR1 |= (1<<20);
  ADC2->SQR3 &= ~(0x1f<<0);
  ADC2->SQR3 |= (4<<0);
  ADC2->SQR3 &= ~(0x1f<<5);
  ADC2->SQR3 |= (5<<5);
  //ADC通用控制寄存器
  ADC->CCR &= ~(3<<16);
  ADC->CCR |= (1<<16);
  
  //ADCx使能
  ADC2->CR2 |= (1<<0);
}

/******************************************
*函数功能  ：获取ADC2通道4、5的转换数据
*函数名    ：ADC2_In4In5_Data
*函数参数  ：void
*函数返回值：u16
*函数描述  ：用于转换光敏电阻数据
*********************************************/
ADC2_DATA ADC2_In4In5_Data(void)
{
  ADC2_DATA data;
  
  //开启ADC2通道转换
  ADC2->CR2 |= (1<<30);
  //等待数据转换完成
  while(!(ADC2->SR & (1<<1)));
  //去取转换完成的ADC数据
  data.adc2_LDR = ADC2->DR;
  //等待数据转换完成
  while(!(ADC2->SR & (1<<1)));
  //去取转换完成的ADC数据
  data.adc2_R45 = ADC2->DR;
  
  return data;
}




