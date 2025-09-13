/*
 * File: Timer.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "Timer.h"
#include "IO.h"
STRTMRegType g_tm_stTimerFlag;
STRTMRegType tm_sTimerFlag;
#define TM_INT_LOAD (SYSCLK_SYS_FREQ / 2000)//2000-500us, 4000-250us
void fnTM_Init(void);
void fnTM_RealTime(void);
void fnTM_Clear(void);

U8 tm_uchTimer2msCount;
U8 tm_uchTimer16msCount;

#define TM_INTERRUPT_TIME_US (U32)500
void fnTM_Init(void)
{
    SysTick->VAL = 0;
    SysTick->LOAD = (TM_INT_LOAD - 1);//48000 for 4MHz, 49152 for 4.096MHZ
    SysTick->CTRL = 0x07;
    memset(&g_tm_stTimerFlag, 0, sizeof(g_tm_stTimerFlag));

}
void fnTM_RealTime(void)
{
    static U16 tm_uchTimer16msCount = 0;
    if(tm_sTimerFlag.Bits.bTimer2ms)
    {
        SysTick->CTRL = 0x00;
        memcpy(&g_tm_stTimerFlag, &tm_sTimerFlag, sizeof(tm_sTimerFlag));
        memset(&tm_sTimerFlag, 0, sizeof(tm_sTimerFlag));
        SysTick->CTRL = 0x07;
    }
    if(g_tm_stTimerFlag.Bits.bTimer16ms)
    {
        tm_uchTimer16msCount ++;
        if(0 == (tm_uchTimer16msCount  & 0x0001))
        {
            g_tm_stTimerFlag.Bits.bTimer32ms = TRUE;
        }
        if(0 == (tm_uchTimer16msCount  & 0x0003)) // * 4
        {
            g_tm_stTimerFlag.Bits.bTimer64ms = TRUE;
        }
        if(0 == (tm_uchTimer16msCount  & 0x003F))//5
        {
            g_tm_stTimerFlag.Bits.bTimer1s = TRUE;
        }
        if(0 == (tm_uchTimer16msCount  & 0x0FFF))//5
        {

            g_tm_stTimerFlag.Bits.bTimer1min = TRUE;
        }

    }
}
void fnTM_Clear(void)
{
    memset(g_tm_stTimerFlag.BYTES, 0, TM_TIMER_NUM);
}

void SysTick_Handler(void)
{
    //4.55US/500US, 4.55US
    static U8 uchTimer500us = 0;
    static U8 uchTimer10mscount = 0;
    static U8 uchTimer100mscounter = 0;

    uchTimer500us ++;
    if(0 == (uchTimer500us & 0x03))
    {
        uchTimer10mscount ++;
        uchTimer100mscounter ++;
        tm_sTimerFlag.Bits.bTimer2ms = TRUE;
        tm_uchTimer2msCount ++;
        if(!(tm_uchTimer2msCount & 0x03))
        {
            tm_sTimerFlag.Bits.bTimer8ms = TRUE;
        }
        if(!(tm_uchTimer2msCount & 0x07))
        {
            tm_sTimerFlag.Bits.bTimer16ms = TRUE;

        }
        if(uchTimer10mscount >= 10)
        {
            tm_sTimerFlag.Bits.bTimer20ms = TRUE;
            uchTimer10mscount = 0;
        }
        if(uchTimer100mscounter >= 50) {
            tm_sTimerFlag.Bits.bTimer100ms = TRUE;
            uchTimer100mscounter = 0;
        }
    }
}

