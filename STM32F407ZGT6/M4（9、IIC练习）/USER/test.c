#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "nvic.h"
#include "exit.h"
#include "delay.h"
#include "at24cxx.h"

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
*         2、AT24C02的0~16用于存储LCD与触摸屏之间的校准系数
***************************************************************/

u8 data = 0;
u8 flay = 0;

int main(void)
{  
  u8 as[] = "885qwertyuiopasd123456789";
  u8 bs[40] ;
  u8 err1,err2;
  
  SysTick_Init();
  NVIC_SetPriorityGrouping(5);       //优先级分组 
  USART1_Init(115200);
  AT24CXX_Init();  
  err1 = AT24CXXaddr_Write_Addr(AT24C02,AT24CXX_WRITE,16,sizeof(as),as);
  err2 = AT24CXXaddr_Read_Addr(AT24C02,AT24CXX_WRITE,16,sizeof(as),bs);
  
  while(1)
  {
    printf("err1 : %d    err2 = %d\r\n",err1,err2);
    printf("bs : %s\r\n",bs);
    delay_ms(1000);
  }
  
}











