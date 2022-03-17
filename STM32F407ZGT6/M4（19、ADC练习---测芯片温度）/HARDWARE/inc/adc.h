#ifndef _ADC_H
#define _ADC_H

  #include "stm32f4xx.h" 
  
  void ADC1_Init_In5(void);
  u16 ADC1_In5_Data(void);
  void ADC2_Init_In4(void);
  u16 ADC2_In4_Data(void);
  void ADC1_Init_In16(void);
  u16 ADC1_In16_Data(void);
  float ADC1_Chip_Temperature(void);
  u16 Get_Adc_Average(u8 ch,u8 times);


#endif












