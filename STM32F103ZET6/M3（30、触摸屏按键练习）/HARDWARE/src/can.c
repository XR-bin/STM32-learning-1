#include "sys.h"  
#include "stdio.h"

/*****************************************************
*函数功能  ：对CAM相应的GPIO口和寄存器进行配置
*函数名    ：CAN1_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
********************************************************/
void CAN1_Init(void)
{
  /*GPIOx配置*/
  
  /*CAN寄存器配置*/
  //CAN时钟使能
  RCC->APB1ENR |= 1<<25;
  /*CAN 控制和状态寄存器*/
  //主控寄存器
  CAN1->MCR |= 1<<6;              //启动自动的总线关闭管理
  CAN1->MCR |= 1<<5;              //启动自动唤醒模式
  CAN1->MCR |= 1<<0;              //CAN硬件进入初始化
  while(!(CAN1->MSR & (1<<0)));   //确认进入初始化模式
  //位时序寄存器
  CAN1->BTR = 0;             //整个寄存器清零
  CAN1->BTR |= 0x80000000;   //静默模式
  CAN1->BTR |= 1<<30;        //环回模式
  CAN1->BTR |= 2<<24;        //允许两个时间片的误差
  //500K ,一下参数是通过计算得到，参考参考手册622
  CAN1->BTR |= (7<<20);      //TS2
	CAN1->BTR |= (11<<16);     //TS1
	CAN1->BTR |= (3<<0);       //tq   时间片宽度
  /*CAN 筛选器寄存器*/
  //筛选器主寄存器
  CAN1->FMR |= 1<<0;        //筛选器进入初始化
  //筛选器模式寄存器
  CAN1->FM1R |= 1<<0;       //设置0号邮箱的筛选器的模式为标识符列表模式
  //筛选器尺度寄存器
  CAN1->FS1R |= 1<<0;       //0号邮箱的筛选器尺度配置为单32位尺度
  //筛选器FIFO分配寄存器
  CAN1->FFA1R &= ~(1<<0);   //0号邮箱的筛选器的消息将存储在0号FIFO中
  //筛选器组i寄存器x(有主从之分)
  CAN1->sFilterRegister[0].FR1 = 0;
  CAN1->sFilterRegister[0].FR1 |= 123<<21;   //0号邮箱的筛选器的主筛选器的ID是123
  CAN1->sFilterRegister[0].FR2 = 0;
  CAN1->sFilterRegister[0].FR2 |= 124<<21;   //0号邮箱的筛选器的主筛选器的ID是124
  //筛选器激活寄存器
  CAN1->FA1R |= 1<<0;       //将0号邮箱的筛选器激
  //筛选器进入工作模式
  CAN1->FMR &= ~(1<<0);
  //CAN硬件退出初始化
  CAN1->MCR &= ~(1<<0);          
}

/******************************************
*函数功能  ：CAN1总线发送一个字节数据
*函数名    ：CAN1_Send_Byte
*函数参数  ：u8 data
*函数返回值：void
*函数描述  ：
*********************************************/
void CAN1_Send_Byte(u8 data)
{
  u8 mailbox = 0xff;
  
  while(mailbox == 0xff)
  {
    if(CAN1->TSR & (1<<26))      mailbox = 0;  //邮箱0是否有空
    else if(CAN1->TSR & (1<<27)) mailbox = 1;  //邮箱1是否有空
    else if(CAN1->TSR & (1<<28)) mailbox = 2;  //邮箱2是否有空
  }
  /*CAN 邮箱寄存器*/
  //发送邮箱标识符寄存器
  CAN1->sTxMailBox[mailbox].TIR = 0;
  CAN1->sTxMailBox[mailbox].TIR |= 123<<21;  //邮箱号
  CAN1->sTxMailBox[mailbox].TIR &= ~(1<<2);     //标准标识符类型
  CAN1->sTxMailBox[mailbox].TIR &= ~(1<<1);    //数据帧类型
  //邮箱数据长度控制和时间戳寄存器
  CAN1->sTxMailBox[mailbox].TDTR = 0;
  CAN1->sTxMailBox[mailbox].TDTR |= 1<<0;    //发送1个字节的数据
  //邮箱数据低位寄存器(只发一个字节所以一个低位寄存器就够了)
  CAN1->sTxMailBox[mailbox].TDLR = 0;
	CAN1->sTxMailBox[mailbox].TDLR = data;      //设置要发送的数据
  //开启发送数据
	CAN1->sTxMailBox[mailbox].TIR |= 1<<0;  
}

/******************************************
*函数功能  ：CAN1总线接收一个字节数据
*函数名    ：CAN1_Receive_Byte
*函数参数  ：void
*函数返回值：void
*函数描述  ：
*********************************************/
void CAN1_Receive_Byte(void)
{
  u8 data;
  //有数据的邮箱个数不为0就连续接收数据
  while(CAN1->RF0R & (3<<0))
  {
    data = CAN1->sFIFOMailBox[0].RDLR;
    //清空0号邮箱的数据，这样才能其他邮箱的数据才能在到0号邮箱，因为是FIFO形式的邮箱
    CAN1->RF0R |= 1<<5; 
    printf("接收到的内容是:%d\r\n",data);
  }
}









