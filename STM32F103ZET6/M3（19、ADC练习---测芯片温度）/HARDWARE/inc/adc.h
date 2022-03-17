#ifndef _ADC_H
#define _ADC_H

  #include "sys.h"
  
  void ADC3_Init_In6(void);
  u16 ADC3_In6_Data(void);
  void ADC1_Init_In16(void);
  u16 ADC1_In16_Data(void);
  double ADC1_Chip_Temperature(void);

#endif







