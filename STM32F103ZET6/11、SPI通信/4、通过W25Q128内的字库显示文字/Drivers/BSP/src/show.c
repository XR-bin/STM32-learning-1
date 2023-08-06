#include "show.h"
#include "w25q128.h"
#include "lcd.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\show.c
*              .\Drivers\BSP\inc\show.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：通过字库显示文字代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：LCD_Show_HZ16
* @brief    ：LCD屏幕显示一个16*16字符/汉字
* @param    ：uint16_t x        (开始的光标x坐标)
* @param    ：uint16_t y        (开始的光标y坐标)
* @param    ：uint8_t *str      (要显示的汉字/字符组)
* @param    ：uint16_t color    (字体颜色)
* @param    ：uint16_t backdrop (背景色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Show_HZ16(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop)
{
    uint32_t n;
    uint32_t flash_addr;
    uint8_t model_ch[16];
    uint8_t model_hz[32];
    uint8_t i,j,temp1;
    uint16_t temp2;
    uint32_t H,L;  //区码、位码

    while(*str != '\0')
    {
        if((32<=*str) && (*str<=127))
        {
            n = *str - 0;
            flash_addr = 0x0b0000 + 0 + n * 16;
            W25Q128_Read_Bytes(flash_addr,16,model_ch);
            for(i=0;i<16;i++)
            {
                temp1 = model_ch[i];
                for(j=0;j<8;j++)
                {
                    if(temp1 & (0x80>>j))
                    {
                        LCD_Point(x+j,y+i,color);
                    }
                    else
                    {
                        LCD_Point(x+j,y+i,backdrop);
                    }
                }
            }
            x += 8;
            str++;
            if(x > (lcddev.width-1-8))
            {
                x=0;
                y+=16;
            }
        }
        else
        {
            H = (*str - 0xa1) * 94;
            L = *(str + 1) - 0xa1;
            n = H + L;
            flash_addr = 0x0b0000 + 0x0000280C + n * 32;
            W25Q128_Read_Bytes(flash_addr,32,model_hz);
            for(i=0;i<16;i++)
            {
                temp2 = (model_hz[i*2]<<8) | (model_hz[i*2+1]);
                for(j=0;j<16;j++)
                {
                    if(temp2 & 0x8000>>j)
                    {
                        LCD_Point(x+j,y+i,color);
                    }
                    else
                    {
                        LCD_Point(x+j,y+i,backdrop);
                    }
                }
            }
            x += 16;
            str += 2;
            if(x > (lcddev.width-1-16))
            {
                x=0;
                y+=16;
            }
        }
    }
}



/**********************************************************
* @funcName ：LCD_Show_HZ32
* @brief    ：LCD屏幕显示一个32*32字符/汉字
* @param    ：uint16_t x        (开始的光标x坐标)
* @param    ：uint16_t y        (开始的光标y坐标)
* @param    ：uint8_t *str      (要显示的汉字/字符组)
* @param    ：uint16_t color    (字体颜色)
* @param    ：uint16_t backdrop (背景色)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void LCD_Show_HZ32(uint16_t x, uint16_t y,uint8_t *str, uint16_t color, uint16_t backdrop)
{
    uint32_t n;
    uint32_t flash_addr;
    uint8_t model_ch[64];
    uint8_t model_hz[128];
    uint8_t i,j,k;
    uint16_t temp1;
    uint32_t temp2;
    uint32_t H,L;  //区码、位码

    while(*str != '\0')
    {
        if((32<=*str) && (*str<=127))
        {
            n = *str - 0;
            flash_addr = 0x0b0000 + 0x00000806 + n * 64;
            W25Q128_Read_Bytes(flash_addr,64,model_ch);
            for(i=0;i<32;i++)
            {
                temp1 = (model_ch[i*2]<<8) | (model_ch[i*2+1]);
                for(j=0;j<16;j++)
                {
                    if(temp1 & (0x8000>>j))
                    {
                        LCD_Point(x+j,y+i,color);
                    }
                    else
                    {
                        LCD_Point(x+j,y+i,backdrop);
                    }
                }
            }
            x += 16;
            str++;
            if(x > (lcddev.width-1-16))
            {
                x=0;
                y+=32;
            }
        }
        else
        {
            H = (*str - 0xa1) * 94;
            L = *(str + 1) - 0xa1;
            n = H + L;
            flash_addr = 0x0b0000 + 0x00042652 + n * 128;
            W25Q128_Read_Bytes(flash_addr,128,model_hz);
            for(i=0;i<32;i++)
            {
                for(k=0;k<4;k++)
                {
                    temp2 <<= 8;
                    temp2 |= model_hz[k+i*4];
                }
                for(j=0;j<32;j++)
                {
                    if(temp2 & 0x80000000>>j)
                    {
                        LCD_Point(x+j,y+i,color);
                    }
                    else
                    {
                        LCD_Point(x+j,y+i,backdrop);
                    }
                }
            }
            x += 32;
            str += 2;
            if(x > (lcddev.width-1-32))
            {
                x=0;
                y+=32;
            }
        }
    }
}


