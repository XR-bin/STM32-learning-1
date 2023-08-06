#ifndef _WWDG_H
#define _WWDG_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /****************    函数外部声明   *****************/
    void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer); /* 初始化窗口看门狗 */
    void WWDG_Set_Counter(uint8_t cnt);                    /* 重设置WWDG计数器的值 */

#endif




