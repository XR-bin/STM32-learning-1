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
 * @brief       外部中断配置函数, 只针对GPIOA~GPIOI
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
    RCC->APB2ENR |= 1 << 14;    /* 使能SYSCFG时钟  */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            offset = (pinpos % 4) * 4;
            SYSCFG->EXTICR[pinpos / 4] &= ~(0x000F << offset);  /* 清除原来设置！！！ */
            SYSCFG->EXTICR[pinpos / 4] |= gpio_num << offset;   /* EXTI.BITx映射到gpiox.bitx */

            EXTI->IMR |= 1 << pinpos;   /* 开启line BITx上的中断(如果要禁止中断，则反操作即可) */

            if (tmode & 0x01) EXTI->FTSR |= 1 << pinpos;        /* line bitx上事件下降沿触发 */

            if (tmode & 0x02) EXTI->RTSR |= 1 << pinpos;        /* line bitx上事件上升降沿触发 */
        }
    }
}

/**
 * @brief       GPIO复用功能选择设置
 * @param       p_gpiox: GPIOA~GPIOI, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       afx:0~15, 代表AF0~AF15.
 *              AF0~15设置情况(这里仅是列出常用的, 详细的请见STM32F407xx数据手册, Table 7):
 *   @arg       AF0:MCO/SWD/SWCLK/RTC       AF1:TIM1/TIM2               AF2:TIM3~5                  AF3:TIM8~11
 *   @arg       AF4:I2C1~I2C3               AF5:SPI1/SPI2/I2S2          AF6:SPI3/I2S3               AF7:USART1~3
 *   @arg       AF8:USART4~6                AF9;CAN1/CAN2/TIM12~14      AF10:USB_OTG/USB_HS         AF11:ETH
 *   @arg       AF12:FSMC/SDIO/OTG_FS       AF13:DCIM                   AF14:                       AF15:EVENTOUT
 * @retval      无
 */
void sys_gpio_af_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t afx)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;;

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            p_gpiox->AFR[pinpos >> 3] &= ~(0X0F << ((pinpos & 0X07) * 4));
            p_gpiox->AFR[pinpos >> 3] |= (uint32_t)afx << ((pinpos & 0X07) * 4);
        }
    }
}

/**
 * @brief       GPIO通用设置
 * @param       p_gpiox: GPIOA~GPIOI, GPIO指针
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
 * @param       ospeed: 0~3; 输出速度, 设置如下:
 *   @arg       SYS_GPIO_SPEED_LOW,  0, 低速
 *   @arg       SYS_GPIO_SPEED_MID,  1, 中速
 *   @arg       SYS_GPIO_SPEED_FAST, 2, 快速
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, 高速
 *
 * @param       pupd: 0~3: 上下拉设置, 设置如下:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, 不带上下拉
 *   @arg       SYS_GPIO_PUPD_PU,   1, 上拉
 *   @arg       SYS_GPIO_PUPD_PD,   2, 下拉
 *   @arg       SYS_GPIO_PUPD_RES,  3, 保留
 *
 * @note:       注意: 在输入模式(普通输入/模拟输入)下, OTYPE和OSPEED参数无效!!
 * @retval      无
 */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            p_gpiox->MODER &= ~(3 << (pinpos * 2)); /* 先清除原来的设置 */
            p_gpiox->MODER |= mode << (pinpos * 2); /* 设置新的模式 */

            if ((mode == 0X01) || (mode == 0X02))   /* 如果是输出模式/复用功能模式 */
            {
                p_gpiox->OSPEEDR &= ~(3 << (pinpos * 2));       /* 清除原来的设置 */
                p_gpiox->OSPEEDR |= (ospeed << (pinpos * 2));   /* 设置新的速度值 */
                p_gpiox->OTYPER &= ~(1 << pinpos) ;             /* 清除原来的设置 */
                p_gpiox->OTYPER |= otype << pinpos;             /* 设置新的输出模式 */
            }

            p_gpiox->PUPDR &= ~(3 << (pinpos * 2)); /* 先清除原来的设置 */
            p_gpiox->PUPDR |= pupd << (pinpos * 2); /* 设置新的上下拉 */
        }
    }
}

/**
 * @brief       设置GPIO某个引脚的输出状态
 * @param       p_gpiox: GPIOA~GPIOI, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
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
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 一次只能读取一个GPIO！
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
 * @param       plln: 主PLL倍频系数(PLL倍频), 取值范围: 64~432.
 * @param       pllm: 主PLL和音频PLL预分频系数(进PLL之前的分频), 取值范围: 2~63.
 * @param       pllp: 主PLL的p分频系数(PLL之后的分频), 分频后作为系统时钟, 取值范围: 2, 4, 6, 8.(仅限这4个值)
 * @param       pllq: 主PLL的q分频系数(PLL之后的分频), 取值范围: 2~15.
 * @note
 *
 *              Fvco: VCO频率
 *              Fsys: 系统时钟频率, 也是主PLL的p分频输出时钟频率
 *              Fq:   主PLL的q分频输出时钟频率
 *              Fs:   主PLL输入时钟频率, 可以是HSI, HSE等.
 *              Fvco = Fs * (plln / pllm);
 *              Fsys = Fvco / pllp = Fs * (plln / (pllm * pllp));
 *              Fq   = Fvco / pllq = Fs * (plln / (pllm * pllq));
 *
 *              外部晶振为 8M的时候, 推荐值: plln = 336, pllm = 8, pllp = 2, pllq = 7.
 *              得到:Fvco = 8 * (336 / 8) = 336Mhz
 *                   Fsys = pll_p_ck = 336 / 2 = 168Mhz
 *                   Fq   = pll_q_ck = 336 / 7 = 48Mhz
 *
 *              F407默认需要配置的频率如下:
 *              CPU频率(HCLK) = pll_p_ck = 168Mhz
 *              AHB1/2/3(rcc_hclk1/2/3) = 168Mhz
 *              APB1(rcc_pclk1) = pll_p_ck / 4 = 42Mhz
 *              APB1(rcc_pclk2) = pll_p_ck / 2 = 84Mhz
 *
 * @retval      错误代码: 0, 成功; 1, HSE错误; 2, PLL1错误; 3, PLL2错误; 4, 切换时钟错误;
 */
uint8_t sys_clock_set(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    uint32_t retry = 0;
    uint8_t retval = 0;
    uint8_t swsval = 0;

    RCC->CR |= 1 << 16; /* HSEON = 1, 开启HSE */

    while (((RCC->CR & (1 << 17)) == 0) && (retry < 0X7FFF))
    {
        retry++;        /* 等待HSE RDY */
    }

    if (retry == 0X7FFF)
    {
        retval = 1;     /* HSE无法就绪 */
    }
    else
    {
        RCC->APB1ENR |= 1 << 28;                /* 电源接口时钟使能 */
        PWR->CR |= 3 << 14;                     /* 高性能模式,时钟可到168Mhz */
        
        RCC->PLLCFGR |= 0X3F & pllm;            /* 设置主PLL预分频系数,  PLLM[5:0]: 2~63 */
        RCC->PLLCFGR |= plln << 6;              /* 设置主PLL倍频系数,    PLLN[8:0]: 192~432 */
        RCC->PLLCFGR |= ((pllp >> 1) - 1) << 16;/* 设置主PLL的p分频系数, PLLP[1:0]: 0~3, 代表2~8分频 */
        RCC->PLLCFGR |= pllq << 24;             /* 设置主PLL的q分频系数, PLLQ[3:0]: 2~15 */
        RCC->PLLCFGR |= 1 << 22;                /* 设置主PLL的时钟源来自HSE */

        RCC->CFGR |= 0 << 4;                    /* HPRE[3:0]  = 0, AHB  不分频, rcc_hclk1/2/3 = pll_p_ck */
        RCC->CFGR |= 5 << 10;                   /* PPRE1[2:0] = 5, APB1 4分频   rcc_pclk1 = pll_p_ck / 4 */
        RCC->CFGR |= 4 << 13;                   /* PPRE2[2:0] = 4, APB2 2分频   rcc_pclk2 = pll_p_ck / 2 */

        RCC->CR |= 1 << 24;                     /* 打开主PLL */

        retry = 0;
        while ((RCC->CR & (1 << 25)) == 0)      /* 等待PLL准备好 */
        {
            retry++;

            if (retry > 0X1FFFFF)
            {
                retval = 2;                     /* 主PLL无法就绪 */
                break;
            }
        }

        FLASH->ACR |= 1 << 8;                   /* 指令预取使能 */
        FLASH->ACR |= 1 << 9;                   /* 指令cache使能 */
        FLASH->ACR |= 1 << 10;                  /* 数据cache使能 */
        FLASH->ACR |= 5 << 0;                   /* 5个CPU等待周期 */
        
        RCC->CFGR |= 2 << 0;                    /* 选择主PLL作为系统时钟 */
        
        retry = 0;
        while (swsval != 3)                     /* 等待成功将系统时钟源切换为pll_p_ck */
        {
            swsval = (RCC->CFGR & 0X0C) >> 2;   /* 获取SWS[1:0]的状态, 判断是否切换成功 */
            retry++;

            if (retry > 0X1FFFFF)
            {
                retval = 4; /* 无法切换时钟 */
                break;
            }
        }
    }

    return retval;
}

/**
 * @brief       系统时钟初始化函数
 * @param       plln: PLL1倍频系数(PLL倍频), 取值范围: 4~512.
 * @param       pllm: PLL1预分频系数(进PLL之前的分频), 取值范围: 2~63.
 * @param       pllp: PLL1的p分频系数(PLL之后的分频), 分频后作为系统时钟, 取值范围: 2~128.(且必须是2的倍数)
 * @param       pllq: PLL1的q分频系数(PLL之后的分频), 取值范围: 1~128.
 * @retval      无
 */
void sys_stm32_clock_init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq)
{
    RCC->CR = 0x00000001;           /* 设置HISON, 开启内部高速RC振荡，其他位全清零 */
    RCC->CFGR = 0x00000000;         /* CFGR清零 */
    RCC->PLLCFGR = 0x00000000;      /* PLLCFGR清零 */
    RCC->CIR = 0x00000000;          /* CIR清零 */
    
    sys_clock_set(plln, pllm, pllp, pllq);  /* 设置时钟 */

    /* 配置中断向量偏移 */
#ifdef  VECT_TAB_RAM
    sys_nvic_set_vector_table(SRAM_BASE, 0x0);
#else
    sys_nvic_set_vector_table(FLASH_BASE, 0x0);
#endif
}







