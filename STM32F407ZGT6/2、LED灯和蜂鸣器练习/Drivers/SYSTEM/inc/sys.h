#ifndef __SYS_H
#define __SYS_H

    /****************   外部头文件声明   ****************/
    #include "stm32f4xx.h"



    /********************   宏定义   ********************/
    /* sys_nvic_ex_config专用宏定义 */
    #define SYS_GPIO_FTIR               1       /* 下降沿触发 */
    #define SYS_GPIO_RTIR               2       /* 上升沿触发 */
    #define SYS_GPIO_BTIR               3       /* 任意边沿触发 */

    /* GPIO设置专用宏定义 */
    #define SYS_GPIO_MODE_IN            0       /* 普通输入模式 */
    #define SYS_GPIO_MODE_OUT           1       /* 普通输出模式 */
    #define SYS_GPIO_MODE_AF            2       /* AF功能模式 */
    #define SYS_GPIO_MODE_AIN           3       /* 模拟输入模式 */

    #define SYS_GPIO_SPEED_LOW          0       /* GPIO速度(低速,2M) */
    #define SYS_GPIO_SPEED_MID          1       /* GPIO速度(中速,25M) */
    #define SYS_GPIO_SPEED_FAST         2       /* GPIO速度(快速,50M) */
    #define SYS_GPIO_SPEED_HIGH         3       /* GPIO速度(高速,100M) */

    #define SYS_GPIO_PUPD_NONE          0       /* 不带上下拉 */
    #define SYS_GPIO_PUPD_PU            1       /* 上拉 */
    #define SYS_GPIO_PUPD_PD            2       /* 下拉 */
    #define SYS_GPIO_PUPD_RES           3       /* 保留 */

    #define SYS_GPIO_OTYPE_PP           0       /* 推挽输出 */
    #define SYS_GPIO_OTYPE_OD           1       /* 开漏输出 */

    /* GPIO引脚位置宏定义  */
    #define SYS_GPIO_PIN0               1<<0
    #define SYS_GPIO_PIN1               1<<1
    #define SYS_GPIO_PIN2               1<<2
    #define SYS_GPIO_PIN3               1<<3
    #define SYS_GPIO_PIN4               1<<4
    #define SYS_GPIO_PIN5               1<<5
    #define SYS_GPIO_PIN6               1<<6
    #define SYS_GPIO_PIN7               1<<7
    #define SYS_GPIO_PIN8               1<<8
    #define SYS_GPIO_PIN9               1<<9
    #define SYS_GPIO_PIN10              1<<10
    #define SYS_GPIO_PIN11              1<<11
    #define SYS_GPIO_PIN12              1<<12
    #define SYS_GPIO_PIN13              1<<13
    #define SYS_GPIO_PIN14              1<<14
    #define SYS_GPIO_PIN15              1<<15



    /****************    函数外部声明   *****************/
    /* 静态函数(仅在sys.c里面用到) */
    static void sys_nvic_priority_group_config(uint8_t group);                      /* 设置NVIC分组 */

    /* 普通函数 */
    void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset);             /* 设置中断偏移量 */
    void sys_nvic_init(uint8_t pprio, uint8_t sprio, uint8_t ch, uint8_t group);    /* 设置NVIC */
    void sys_nvic_ex_config(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t tmode);   /* 外部中断配置函数,只针对GPIOA~GPIOK */
    void sys_gpio_af_set(GPIO_TypeDef *gpiox, uint16_t pinx, uint8_t afx);          /* GPIO复用功能选择设置  */
    void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, 
                  uint32_t otype, uint32_t ospeed, uint32_t pupd);              /*  GPIO通用设置 */

    void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status);    /* 设置GPIO某个引脚的输出状态 */
    uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx);                 /* 读取GPIO某个引脚的状态 */
    void sys_standby(void);         /* 进入待机模式 */
    void sys_soft_reset(void);      /* 系统软复位 */

    uint8_t sys_clock_set(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq);      /* 时钟设置函数 */
    void sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq);  /* 系统时钟初始化函数 */
    void sys_qspi_enable_memmapmode(uint8_t ftype); /* QSPI进入内存映射模式 */

    /* 以下为汇编函数 */
    void sys_wfi_set(void);             /* 执行WFI指令 */
    void sys_intx_disable(void);        /* 关闭所有中断 */
    void sys_intx_enable(void);         /* 开启所有中断 */
    void sys_msr_msp(uint32_t addr);    /* 设置栈顶地址 */

#endif











