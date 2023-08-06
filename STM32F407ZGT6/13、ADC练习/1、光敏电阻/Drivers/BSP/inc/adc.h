#ifndef _ADC_H
#define _ADC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void ADC3_In5_Init(void);       /* ADC3初始化 */
    uint16_t ADC3_In5_Data(void);   /* 获取ADC3转换数值 */
    uint16_t Oversampling(void);    /* 过采样 */

#endif



