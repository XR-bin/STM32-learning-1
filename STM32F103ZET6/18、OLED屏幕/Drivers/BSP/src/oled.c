#include "oled.h"
#include "iic.h"
#include "matrix.h"



/**********************************************************
* @funcName ：OLED_Write_Command
* @brief    ：对OLE屏发送指令
* @param    ：uint8_t cmd (指令)
* @retval   ：uint8_t    0---发生成功   1---发生失败
* @details  ：
* @fn       ：
************************************************************/
uint8_t OLED_Write_Command(uint8_t cmd)
{
    IIC_Start();             /* 起始信号 */
    IIC_Send_Byte(0x78);     /* 发送OLED器件地址，最低位表示读写位，0为写，1为读 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(0x00);         /* 0x00表示这次发送是指令 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(cmd);         /* 发送指令 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Stop();
    return 1;
}



/**********************************************************
* @funcName ：OLED_Write_Data
* @brief    ：对OLE屏发送数据
* @param    ：uint8_t data (数据)
* @retval   ：uint8_t    0---发生成功   1---发生失败
* @details  ：
* @fn       ：
************************************************************/
uint8_t OLED_Write_Data(uint8_t data)
{
    IIC_Start();             /* 起始信号 */
    IIC_Send_Byte(0x78);     /* 发送OLED器件地址，最低位表示读写位，0为写，1为读 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(0x40);         /* 0x40表示这次发送是数据 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(data);         /* 发送指令 */

    /* 判断应答 */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Stop();
    return 1;
}



/**********************************************************
* @funcName ：OLED_WR_Byte
* @brief    ：对OLE屏发送指令/数据
* @param    ：uint8_t Byte (指令值/数据值)
* @param    ：uint8_t DC (选择发生的值是指令还是数据)
*   @arg    ：0 --- 指令   1 --- 数据
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void OLED_WR_Byte(uint8_t Byte, uint8_t DC)
{
    if(DC) OLED_Write_Data(Byte);
    else OLED_Write_Command(Byte);
}



/**********************************************************
* @funcName ：OLED_Clear
* @brief    ：对OLE屏清屏(清屏后屏幕是暗的)
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*             0x00~0x0f:  设置8位起始列地址的低四位。
*             0x10~0x1f:  设置8位起始列地址的高四位。
*             0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
************************************************************/
void OLED_Clear(void)
{
    uint8_t i,j;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);   /* 选择0+i页(控制8*i ~ 8*(i+1)-1行) */
        OLED_WR_Byte(0x00,OLED_CMD);     /* 设置显示位置—列低地址 */
        OLED_WR_Byte(0x10,OLED_CMD);     /* 设置显示位置—列高地址  从第0列开始 */
        for(j=0;j<128;j++) OLED_WR_Byte(0x00,OLED_DATA); /* 一个字节控制8行1列 */
    }
}



/**********************************************************
* @funcName ：OLED_XY
* @brief    ：设置显示光标的坐标
* @param    ：uint8_t x (起始光标x坐标)
* @param    ：uint8_t y (起始光标y坐标)
* @retval   ：void
* @details  ：
* @fn       ：
*             0x00~0x0f:  设置8位起始列地址的低四位。
*             0x10~0x1f:  设置8位起始列地址的高四位。
*             0xb0~0xb7:  设置页地址（共8页，每页8个像素点），本款OLED有128*64个像素点
*        注意：  
*              这里说的页指的是行，一页控制八行，一共有8页(64行)
*              这里的列有两个指令控制，分别控制低四位和高四位，一共有128列
*              发送数据时，一个字节数据控制8行1列的状态
************************************************************/
void OLED_XY(uint8_t x,uint8_t y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);           /* 设置哪一页(哪八行) */
    OLED_WR_Byte(0x00|(x&0x0f),OLED_CMD);    /* 设置哪一列的低8位 */
    OLED_WR_Byte(0x10|(x>>4),OLED_CMD);      /* 设置哪一列的高8位 */
}



/**********************************************************
* @funcName ：OLED_Open
* @brief    ：启动OLED屏
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*             0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*             0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
************************************************************/
void OLED_Open(void)
{
    /* 开启电荷泵 */
    OLED_WR_Byte(0x8D,OLED_CMD);
    OLED_WR_Byte(0x14,OLED_CMD);
    /* 让OLED进入正常工作模式 */
    OLED_WR_Byte(0xAF,OLED_CMD);
}



/**********************************************************
* @funcName ：OLED_Close
* @brief    ：关闭OLED屏
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*             0x8D指令后接0X14或者0x10:  开启或者关闭电荷泵。
*             0xAE或0xAF:  设置屏幕显示关（睡眠模式）或者显示开（正常模式）
************************************************************/
void OLED_Close(void)
{
    /* 关闭电荷泵 */
    OLED_WR_Byte(0x8D,OLED_CMD);
    OLED_WR_Byte(0x10,OLED_CMD);
    /* 让OLED进入睡眠模式 */
    OLED_WR_Byte(0xAE,OLED_CMD);
}



/**********************************************************
* @funcName ：OLED_Init
* @brief    ：OLED正式初始化
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void OLED_Init(void)
{
    /* GPIOx初始化 */
    IIC_Init();

    delay_ms(5);

    /*厂家代码*/
    OLED_WR_Byte(0xAE,OLED_CMD);    /* 关闭显示 */
    OLED_WR_Byte(0x00,OLED_CMD);    /* 设置低列地址 */
    OLED_WR_Byte(0x10,OLED_CMD);    /* 设置高列地址 */
    OLED_WR_Byte(0x40,OLED_CMD);    /* 设置起始行地址 */
    OLED_WR_Byte(0xB0,OLED_CMD);    /* 设置页地址 */

    OLED_WR_Byte(0x81,OLED_CMD);    /* 对比度设置，可设置亮度 */
    OLED_WR_Byte(0xFF,OLED_CMD);    /* 265 */

    OLED_WR_Byte(0xA1,OLED_CMD);    /* 设置段(SEG)的起始映射地址 */
    OLED_WR_Byte(0xA6,OLED_CMD);    /* 正常显示；0xa7逆显示 */

    OLED_WR_Byte(0xA8,OLED_CMD);    /* 设置驱动路数（16~64） */
    OLED_WR_Byte(0x3F,OLED_CMD);    /* 64duty */

    OLED_WR_Byte(0xC8,OLED_CMD);    /* 重映射模式，COM[N-1]~COM0扫描 */

    OLED_WR_Byte(0xD3,OLED_CMD);    /* 设置显示偏移 */
    OLED_WR_Byte(0x00,OLED_CMD);    /* 无偏移 */

    OLED_WR_Byte(0xD5,OLED_CMD);    /* 设置震荡器分频 */
    OLED_WR_Byte(0x80,OLED_CMD);    /* 使用默认值 */

    OLED_WR_Byte(0xD9,OLED_CMD);    /* 设置 Pre-Charge Period */
    OLED_WR_Byte(0xF1,OLED_CMD);    /* 使用官方推荐值 */

    OLED_WR_Byte(0xDA,OLED_CMD);    /* 设置 com pin configuartion */
    OLED_WR_Byte(0x12,OLED_CMD);    /* 使用默认值 */

    OLED_WR_Byte(0xDB,OLED_CMD);    /* 设置 Vcomh，可调节亮度（默认） */
    OLED_WR_Byte(0x40,OLED_CMD);    /* 使用官方推荐值 */

    OLED_WR_Byte(0x8D,OLED_CMD);    /* 设置OLED电荷泵 */
    OLED_WR_Byte(0x14,OLED_CMD);    /* 开显示 */

    OLED_WR_Byte(0xAF,OLED_CMD);    /* 开启OLED面板显示 */

    /*清屏*/
    OLED_Clear();
    /*初始坐标*/
    OLED_XY(0,0);
}



/**********************************************************
* @funcName ：OLED_ShowChar16
* @brief    ：OLED显示一个16*16的字符
* @param    ：uint16_t x  (起始位置x坐标)
* @param    ：uint16_t y  (起始位置y坐标)
* @param    ：uint16_t ch (要显示的字符)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void OLED_ShowChar16(uint16_t x,uint8_t y, uint8_t ch)
{
    uint8_t i,c;

    /* 计算字符偏移量 */
    c = ch - ' ';

    /* 判断是否超出列数 */
    if(x+16 > (OLED_LINE-1))
    {
        y=y+(16/8)*((x+16)/(OLED_LINE-1));   /* y值不要超过7 */
        if((x+16-(OLED_LINE-1))<16)x=0;
        else x=(x+16-(OLED_LINE-1))/16*16;
    }

    /* 注意：16的字符由2页组成 */
    /* 确定坐标 */
    OLED_XY(x,y);
    /* 显示这一页的字符数据(1页=8行) */
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i],OLED_DATA);
    /* 确定坐标 */
    OLED_XY(x,y+1);
    /* 显示这一页的字符数据(1页=8行) */
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i+8],OLED_DATA);
}



/**********************************************************
* @funcName ：OLED_ShowString16
* @brief    ：OLED显示一个16*16的字符串
* @param    ：uint16_t x    (起始位置x坐标)
* @param    ：uint16_t y    (起始位置y坐标)
* @param    ：uint16_t *str (要显示的字符串)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void OLED_ShowString16(uint8_t x,uint8_t y,uint8_t *str)
{
    uint8_t i=0;

    while(*str != '\0')
    {
        OLED_ShowChar16(x+i*(16/2),y,*str);
        str++;
        i++;
    }
}


