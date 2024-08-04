#ifndef __FSMC_H
#define __FSMC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************     结构体定义     ****************/
    /* 地址的偏离多少取决于数据类型 */
    /* uint8_t   偏移0x1   uint16_t  偏移0x2     uint32_t  偏移0x4 */
    typedef struct
    {                                  /* 基础地址      0x6c00 0000 */
        volatile uint16_t LCD_CMD;     /* 命令数据地址  0x6c00 07fe   地址偏移0x00 */
        volatile uint16_t LCD_DAT;     /* 参数数据地址  0x6c00 0800   地址偏移0x02 */
    }LCD_TypeDef;



    /********************   宏定义   ********************/
    /***
    *注意：FSMC的存储区域的Bank1的范围为0x6000 0000
    *      
    *      Bank1的区域1的范围：0x6000 0000 ~ 0x63ff ffff
    *      Bank1的区域2的范围：0x6400 0000 ~ 0x67ff ffff
    *      Bank1的区域3的范围：0x6800 0000 ~ 0x6bff ffff
    *      Bank1的区域4的范围：0x6c00 0000 ~ 0x6bff ffff
    *
    *      A6控制发送的是数据还是命令(对于LCD屏)，M4本应该是第6位(二进制)的1、0控制
    *   但我们选择了16位，则CUP发送的地址应该是从第1位开始(0号位保留)，而不是
    *   第0位开始,但相对与LCD和FSMC收到的数据依然是0x6c00 4000，但我们写时应该写
    *   0x6c00 8000才是正确的
    */
    /* 定义方法1(宏定义方法) */
    #define  LCD_CMD1     (*(volatile uint16_t *)((u32)0x6c000000))
    #define  LCD_DAT1     (*(volatile uint16_t *)((u32)0x6c000800))

    /***
    *这里要注意：
    *          这里的7e是相对于M4内部的，对于FSMC和LCD来说是3f
    *          对于M4内部只需关注第7位(二进制)    7e == 0111 1110
    *          对于FSMC和LCD只需关注第6位(二进制) 3f == 0011 1111
    *          对于LCD来说第6位为0是命令数据地址、1是参数数据地址
    *          然后通过结构的偏移来实现地址的转变，不懂可以看LCD_TypeDef
    *          的定义
    */
    /* 定义方法2(结构体方法) */
    #define LCD_BASE  ((uint32_t)0x6c0007fe)     /* 基础地址 */
    #define LCD       ((LCD_TypeDef *) LCD_BASE)



    /****************    函数外部声明   *****************/
    void FSMC_LCD_Init(void);          /* FSMC初始化 */

#endif









