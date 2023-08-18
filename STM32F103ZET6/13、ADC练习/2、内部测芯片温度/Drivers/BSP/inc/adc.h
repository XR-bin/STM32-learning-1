#ifndef __ADC_H
#define __ADC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void ADC1_In16_Init(void);            /* ADC1初始化 */
    uint16_t ADC1_In16_Data(void);        /* 获取ADC1转换值 */
    uint16_t Oversampling(void);          /* 过采样 */
    double ADC1_Chip_Temperature1(void);  /* 将ADC值转换为芯片温度值(非过采样) */
    double ADC1_Chip_Temperature2(void);  /* 将ADC值转换为芯片温度值(过采样) */

#endif
