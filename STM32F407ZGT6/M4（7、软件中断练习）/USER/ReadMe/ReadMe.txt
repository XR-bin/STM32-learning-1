关于程序的注意事项及版本信息及修改信息及说明


void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
 函数名：NVIC_SetPriorityGrouping
 函数返回值： 无
 函数参数  ： uint32_t PriorityGroup   //你要设置的抢占优先级的位数
 函数功能  :  设置中断的优先级分组， 分配抢占和响应的位数
 
uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
 函数名：NVIC_EncodePriority
 作  用：将优先级分组、抢占优先级值、响应优先级值、计算成一个整型数据返回
 参  数：优先级分组(寄存器的 8~10 位的写值)
		 抢占优先级值（抢占优先级值）
		 响应优先级值（响应优先级值）  

void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
 函数名：NVIC_SetPriority
 作  用：设置具体某个中断源的优先级(编码)
 参  数：IRQn_Type IRQn ----具体中断的编号（可以直接写名字）     
		 priority-----------计算出来的中断优先级编码
		 
void NVIC_EnableIRQ(IRQn_Type IRQn)
 函数名：NVIC_EnableIRQ
 作  用：NVIC模块响应片上外设中断源
 参  数：IRQn_Type IRQn ----具体中断的编号（可以直接写名字）
	

中断相关函数备份
    NVIC_SetPriorityGrouping(7-2);       //优先级分组                      1435
	pri = NVIC_EncodePriority (5, 1, 2); //计算优先级编码值                1592
	NVIC_SetPriority(USART1_IRQn,pri);   //将编码值写入具体中断源          1550
	NVIC_EnableIRQ(USART1_IRQn);         //使能NVIC响应通道                1467