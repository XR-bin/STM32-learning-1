#include "sys.h"



/**
 * @brief       设置中断向量表偏移地址
 * @param       baseaddr: 基址
 * @param       offset: 偏移量(必须是0, 或者0X100的倍数)
 * @retval      无
 */
void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset)
{
    /* 设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留 */
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}

/**
 * @brief       设置NVIC分组
 * @param       group: 0~4,共5组, 详细解释见: sys_nvic_init函数参数说明
 * @retval      无
 */
static void sys_nvic_priority_group_config(uint8_t group)
{
    uint32_t temp, temp1;
    temp1 = (~group) & 0x07;/* 取后三位 */
    temp1 <<= 8;
    temp = SCB->AIRCR;      /* 读取先前的设置 */
    temp &= 0X0000F8FF;     /* 清空先前分组 */
    temp |= 0X05FA0000;     /* 写入钥匙 */
    temp |= temp1;
    SCB->AIRCR = temp;      /* 设置分组 */
}

/**
 * @brief       设置NVIC(包括分组/抢占优先级/子优先级等)
 * @param       pprio: 抢占优先级(PreemptionPriority)
 * @param       sprio: 子优先级(SubPriority)
 * @param       ch: 中断编号(Channel)
 * @param       group: 中断分组
 *   @arg       0, 组0: 0位抢占优先级, 4位子优先级
 *   @arg       1, 组1: 1位抢占优先级, 3位子优先级
 *   @arg       2, 组2: 2位抢占优先级, 2位子优先级
 *   @arg       3, 组3: 3位抢占优先级, 1位子优先级
 *   @arg       4, 组4: 4位抢占优先级, 0位子优先级
 * @note        注意优先级不能超过设定的组的范围! 否则会有意想不到的错误
 * @retval      无
 */
void sys_nvic_init(uint8_t pprio, uint8_t sprio, uint8_t ch, uint8_t group)
{
    uint32_t temp;
    sys_nvic_priority_group_config(group);  /* 设置分组 */
    temp = pprio << (4 - group);
    temp |= sprio & (0x0f >> group);
    temp &= 0xf;                            /* 取低四位 */
    NVIC->ISER[ch / 32] |= 1 << (ch % 32);  /* 使能中断位(要清除的话,设置ICER对应位为1即可) */
    NVIC->IP[ch] |= temp << 4;              /* 设置响应优先级和抢断优先级 */
}

/**
 * @brief       外部中断配置函数, 只针对GPIOA~GPIOG
 * @note        该函数会自动开启对应中断, 以及屏蔽线
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       tmode: 1~3, 触发模式
 *   @arg       SYS_GPIO_FTIR, 1, 下降沿触发
 *   @arg       SYS_GPIO_RTIR, 2, 上升沿触发
 *   @arg       SYS_GPIO_BTIR, 3, 任意电平触发
 * @retval      无
 */
void sys_nvic_ex_config(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t tmode)
{
    uint8_t offset;
    uint32_t gpio_num = 0;      /* gpio编号, 0~10, 代表GPIOA~GPIOG */
    uint32_t pinpos = 0, pos = 0, curpin = 0;

    gpio_num = ((uint32_t)p_gpiox - (uint32_t)GPIOA) / 0X400 ;/* 得到gpio编号 */
    RCC->APB2ENR |= 1 << 0;     /* AFIO = 1,使能AFIO时钟 */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            offset = (pinpos % 4) * 4;
            AFIO->EXTICR[pinpos / 4] &= ~(0x000F << offset);    /* 清除原来设置！！！ */
            AFIO->EXTICR[pinpos / 4] |= gpio_num << offset;     /* EXTI.BITx映射到gpiox.bitx */

            EXTI->IMR |= 1 << pinpos;   /* 开启line BITx上的中断(如果要禁止中断，则反操作即可) */

            if (tmode & 0x01) EXTI->FTSR |= 1 << pinpos;        /* line bitx上事件下降沿触发 */

            if (tmode & 0x02) EXTI->RTSR |= 1 << pinpos;        /* line bitx上事件上升降沿触发 */
        }
    }
}

/**
 * @brief       GPIO重映射功能选择设置
 *   @note      这里仅支持对MAPR寄存器的配置, 不支持对MAPR2寄存器的配置!!!
 * @param       pos: 在AFIO_MAPR寄存器里面的起始位置, 0~24
 *   @arg       [0]    , SPI1_REMAP;         [1]    , I2C1_REMAP;         [2]    , USART1_REMAP;        [3]    , USART2_REMAP;
 *   @arg       [5:4]  , USART3_REMAP;       [7:6]  , TIM1_REMAP;         [9:8]  , TIM2_REMAP;          [11:10], TIM3_REMAP;
 *   @arg       [12]   , TIM4_REMAP;         [14:13], CAN_REMAP;          [15]   , PD01_REMAP;          [16]   , TIM15CH4_REMAP;
 *   @arg       [17]   , ADC1_ETRGINJ_REMAP; [18]   , ADC1_ETRGREG_REMAP; [19]   , ADC2_ETRGINJ_REMAP;  [20]   , ADC2_ETRGREG_REMAP;
 *   @arg       [26:24], SWJ_CFG;
 * @param       bit: 占用多少位, 1 ~ 3, 详见pos参数说明
 * @param       val: 要设置的复用功能, 0 ~ 4, 得根据pos位数决定, 详细的设置值, 参见: <<STM32中文参考手册 V10>> 8.4.2节, 对MAPR寄存器的说明
 *              如: sys_gpio_remap_set(24, 3, 2); 则是设置SWJ_CFG[2:0]    = 2, 选择关闭JTAG, 开启SWD.
 *                  sys_gpio_remap_set(10, 2, 2); 则是设置TIM3_REMAP[1:0] = 2, TIM3选择部分重映射, CH1->PB4, CH2->PB5, CH3->PB0, CH4->PB1
 * @retval      无
 */
void sys_gpio_remap_set(uint8_t pos, uint8_t bit, uint8_t val)
{
    uint32_t temp = 0;
    uint8_t i = 0;
    RCC->APB2ENR |= 1 << 0;     /* 开启辅助时钟 */

    for (i = 0; i < bit; i++)   /* 填充bit个1 */
    {
        temp <<= 1;
        temp += 1;
    }

    AFIO->MAPR &= ~(temp << pos);       /* 清除MAPR对应位置原来的设置 */
    AFIO->MAPR |= (uint32_t)val << pos; /* 设置MAPR对应位置的值 */
}

/**
 * @brief       GPIO通用设置
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 *
 * @param       mode: 0~3; 模式选择, 设置如下:
 *   @arg       SYS_GPIO_MODE_IN,  0, 输入模式(系统复位默认状态)
 *   @arg       SYS_GPIO_MODE_OUT, 1, 输出模式
 *   @arg       SYS_GPIO_MODE_AF,  2, 复用功能模式
 *   @arg       SYS_GPIO_MODE_AIN, 3, 模拟输入模式
 *
 * @param       otype: 0 / 1; 输出类型选择, 设置如下:
 *   @arg       SYS_GPIO_OTYPE_PP, 0, 推挽输出
 *   @arg       SYS_GPIO_OTYPE_OD, 1, 开漏输出
 *
 * @param       ospeed: 0~2; 输出速度, 设置如下(注意: 不能为0!!):
 *   @arg       SYS_GPIO_SPEED_LOW,  2, 低速
 *   @arg       SYS_GPIO_SPEED_MID,  1, 中速
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, 高速
 *
 * @param       pupd: 0~3: 上下拉设置, 设置如下:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, 不带上下拉
 *   @arg       SYS_GPIO_PUPD_PU,   1, 上拉
 *   @arg       SYS_GPIO_PUPD_PD,   2, 下拉
 *   @arg       SYS_GPIO_PUPD_RES,  3, 保留
 *
 * @note:       注意:
 *              1, 在输入模式(普通输入/模拟输入)下, otype 和 ospeed 参数无效!!
 *              2, 在输出模式下, pupd 参数无效!!(开漏输出无法使用内部上拉电阻!!)
 * @retval      无
 */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;
    uint32_t config = 0;        /* 用于保存某一个IO的设置(CNF[1:0] + MODE[1:0]),只用了其最低4位 */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            config = 0;         /* bit0~3都设置为0, 即CNF[1:0] = 0; MODE[1:0] = 0;  默认是模拟输入模式 */

            if ((mode == 0X01) || (mode == 0X02))   /* 如果是普通输出模式/复用功能模式 */
            {
                config = ospeed & 0X03;             /* 设置bit0/1 MODE[1:0] = 2/1/3 速度参数 */
                config |= (otype & 0X01) << 2;      /* 设置bit2   CNF[0]    = 0/1   推挽/开漏输出 */
                config |= (mode - 1) << 3;          /* 设置bit3   CNF[1]    = 0/1   普通/复用输出 */
            }
            else if (mode == 0)     /* 如果是普通输入模式 */
            {
                if (pupd == 0)   /* 不带上下拉,即浮空输入模式 */
                {
                    config = 1 << 2;               /* 设置bit2/3 CNF[1:0] = 01   浮空输入模式 */
                }
                else
                {
                    config = 1 << 3;                            /* 设置bit2/3 CNF[1:0] = 10   上下拉输入模式 */
                    p_gpiox->ODR &= ~(1 << pinpos);             /* 清除原来的设置 */
                    p_gpiox->ODR |= (pupd & 0X01) << pinpos;    /* 设置ODR = 0/1 下拉/上拉 */
                }
            }

            /* 根据IO口位置 设置CRL / CRH寄存器 */
            if (pinpos <= 7)
            {
                p_gpiox->CRL &= ~(0X0F << (pinpos * 4));        /* 清除原来的设置 */
                p_gpiox->CRL |= config << (pinpos * 4);         /* 设置CNFx[1:0] 和 MODEx[1:0], x = pinpos = 0~7 */
            }
            else
            {
                p_gpiox->CRH &= ~(0X0F << ((pinpos - 8) * 4));  /* 清除原来的设置 */
                p_gpiox->CRH |= config << ((pinpos - 8) * 4);   /* 设置CNFx[1:0] 和 MODEx[1:0], x = pinpos = 8~15 */

            }
        }
    }
}

/**
 * @brief       设置GPIO某个引脚的输出状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, 引脚状态(仅最低位有效), 设置如下:
 *   @arg       0, 输出低电平
 *   @arg       1, 输出高电平
 * @retval      无
 */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* 设置GPIOx的pinx为1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* 设置GPIOx的pinx为0 */
    }
}

/**
 * @brief       读取GPIO某个引脚的状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @retval      返回引脚状态, 0, 低电平; 1, 高电平
 */
uint8_t sys_gpio_pin_get(GPIO_TypeDef *p_gpiox, uint16_t pinx)
{
    if (p_gpiox->IDR & pinx)
    {
        return 1;   /* pinx的状态为1 */
    }
    else
    {
        return 0;   /* pinx的状态为0 */
    }
}

/**
 * @brief       执行: WFI指令(执行完该指令进入低功耗状态, 等待中断唤醒)
 * @param       无
 * @retval      无
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       关闭所有中断(但是不包括fault和NMI中断)
 * @param       无
 * @retval      无
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       开启所有中断
 * @param       无
 * @retval      无
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       设置栈顶地址
 * @note        左侧的红X, 属于MDK误报, 实际是没问题的
 * @param       addr: 栈顶地址
 * @retval      无
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);    /* 设置栈顶地址 */
}

/**
 * @brief       进入待机模式
 * @param       无
 * @retval      无
 */
void sys_standby(void)
{
    RCC->APB1ENR |= 1 << 28;    /* 使能电源时钟 */
    PWR->CSR |= 1 << 8;         /* 设置WKUP用于唤醒 */
    PWR->CR |= 1 << 2;          /* 清除WKUP 标志 */
    PWR->CR |= 1 << 1;          /* PDDS = 1, 允许进入深度睡眠模式(PDDS) */
    SCB->SCR |= 1 << 2;         /* 使能SLEEPDEEP位 (SYS->CTRL) */
    sys_wfi_set();              /* 执行WFI指令, 进入待机模式 */
}

/**
 * @brief       系统软复位
 * @param       无
 * @retval      无
 */
void sys_soft_reset(void)
{
    SCB->AIRCR = 0X05FA0000 | (uint32_t)0x04;
}

/**
 * @brief       时钟设置函数
 * @param       plln: PLL倍频系数(PLL倍频), 取值范围: 2~16
 * @note
 *
 *              PLLCLK: PLL输出时钟
 *              PLLSRC: PLL输入时钟频率, 可以是 HSI/2, HSE/2, HSE等, 一般选择HSE.
 *              SYSCLK: 系统时钟, 可选来自 HSI/PLLCLK/HSE, 一般选择来自PLLCLK
 *              FCLK  : Cortex M3内核时钟, 等于HCLK
 *              HCLK  : AHB总线时钟, 来自 SYSCLK 的分频, 可以是1...512分频, 一般不分频
 *              PCLK2 : APB2总线时钟, 来自 HCLK 的分频(最大72Mhz), 可以是1/2/4/8/16分频, 一般不分频
 *              PCLK1 : APB1总线时钟, 来自 HCLK 的分频(最大36Mhz), 可以是1/2/4/8/16分频, 一般二分频
 *
 *              PLLCLK = PLLSRC * plln;
 *              FCLK = HCLK = SYSCLK;
 *              PCLK2 = HCLK;
 *              PCLK1 = HCLK / 2;
 *
 *              我们一般选择PLLSRC来自HSE, 即来自外部晶振.
 *              当外部晶振为 8M的时候, 推荐: plln = 9, AHB不分频, 得到:
 *              PLLCLK = 8 * 9 = 72Mhz
 *              FCLK = HCLK = SYSCLK = PLLCLK / 1 = 72Mhz
 *              PCLK2 = HCLK = 72Mhz
 *              PCLK1 = HCLK / 2 = 36Mhz
 *
 *              关于STM32F103的PLL说明详见: <<STM32中文参考手册 V10>>第六章相关内容
 *
 * @retval      错误代码: 0, 成功; 1, HSE错误;
 */
uint8_t sys_clock_set(uint32_t plln)
{
    uint32_t retry = 0;
    uint8_t retval = 0;
    RCC->CR |= 0x00010000;          /* 外部高速时钟使能HSEON */

    while (retry < 0XFFF0)
    {
        __nop();

        /* 注意, MDK5.29或以后版本, 在使能HSEON以后, 如果不加一定的延时
         * 再开始其他配置, 会导致仿真器下载完代码, 延时函数运行不正常的 bug
         * 需要按复位按键, 延时才会正常, 在这里加一定的延时, 可以解决这个 bug
         * 这里, 我们设置的延时时间, 至少是 0X8000 个 nop时间
         */
        if (RCC->CR & (1 << 17) && retry > 0X8000)
        {
            break;
        }

        retry++;        /* 等待HSE RDY */
    }

    if (retry >= 0XFFF0)
    {
        retval = 1;     /* HSE无法就绪 */
    }
    else
    {
        RCC->CFGR = 0X00000400;     /* PCLK1 = HCLK / 2; PCLK2 = HCLK; HCLK = SYSCLK; */
        plln -= 2;                  /* 抵消2个单位(因为是从2开始的, 设置0就是2) */
        RCC->CFGR |= plln << 18;    /* 设置PLL值 2~16 */
        RCC->CFGR |= 1 << 16;       /* PLLSRC = 1, 选择 HSE 作为 PLL 输入时钟 */

        /* FLASH_ACR寄存器的描述详见: <<STM32F10xx闪存编程手册>> */
        FLASH->ACR = 1 << 4;        /* PRFTBE = 1 开启预取缓冲区 */
        FLASH->ACR |= 2 << 0;       /* LATENCY[2:0] = 2 FLASH两个等待周期 */

        RCC->CR |= 1 << 24;         /* PLLON = 1, 使能PLL */

        while (!(RCC->CR >> 25));   /* 等待PLL锁定 */

        RCC->CFGR |= 2 << 0;        /* SW[1:0] = 2, 选择PLL输出作为系统时钟 */

        while (((RCC->CFGR >> 2) & 0X03) != 2); /* 等待PLL作为系统时钟设置成功 */
    }

    return retval;
}

/**
 * @brief       系统时钟初始化函数
 * @param       plln: PLL倍频系数(PLL倍频), 取值范围: 2~16
 * @retval      无
 */
void sys_stm32_clock_init(uint32_t plln)
{
    RCC->APB1RSTR = 0x00000000;     /* 复位结束 */
    RCC->APB2RSTR = 0x00000000;
    
    RCC->AHBENR = 0x00000014;       /* 睡眠模式闪存和SRAM时钟使能.其他关闭 */
    RCC->APB2ENR = 0x00000000;      /* 外设时钟关闭 */
    RCC->APB1ENR = 0x00000000;
    
    RCC->CR |= 0x00000001;          /* 使能内部高速时钟HSION */
    RCC->CFGR &= 0xF8FF0000;        /* 复位SW[1:0], SWS[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0], MCO[2:0] */
    RCC->CR &= 0xFEF6FFFF;          /* 复位HSEON, CSSON, PLLON */
    RCC->CR &= 0xFFFBFFFF;          /* 复位HSEBYP */
    RCC->CFGR &= 0xFF80FFFF;        /* 复位PLLSRC, PLLXTPRE, PLLMUL[3:0] 和 USBPRE/OTGFSPRE */
    RCC->CIR = 0x009F0000;          /* 关闭所有RCC中断并清除中断标志 */

    sys_clock_set(plln);            /* 设置时钟 */

    /* 配置中断向量偏移 */
#ifdef  VECT_TAB_RAM
    sys_nvic_set_vector_table(SRAM_BASE, 0x0);
#else
    sys_nvic_set_vector_table(FLASH_BASE, 0x0);
#endif
}









