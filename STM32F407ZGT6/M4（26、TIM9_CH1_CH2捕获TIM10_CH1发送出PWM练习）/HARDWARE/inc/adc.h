#ifndef _ADC_H
#define _ADC_H

  #include "stm32f4xx.h" 
  
  typedef struct adc2_data
  {
    u16 adc2_LDR;
    u16 adc2_R45;
  }ADC2_DATA;
  
  void ADC1_Init_In5(void);
  void ADC1_Init2_In5(void);
  u16 ADC1_In5_Data(void);
  void ADC2_Init_In4(void);
  u16 ADC2_In4_Data(void);
  void ADC1_Init_In16(void);
  u16 ADC1_In16_Data(void);
  float ADC1_Chip_Temperature(void);
  void ADC2_Init_In4In5(void);
  ADC2_DATA ADC2_In4In5_Data(void);

#endif












