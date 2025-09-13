/*
 * File: Switch.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "Switch.h"
#include "Timer.h"
#include "App.h"
#include "IO.h"
#include "App.h"
#include "spwm.h"
#define SW_DEBOUNCE_TIMES (10)
STRSWConfig g_sw_stConfig[SW_SWITCH_NUM] =
{


    {P_StartUp_PORT, P_StartUp_PIN, 10, 0}


};
SWStatusType g_sw_stRegs;
U8 uchStartLevel;
void fnSW_Init(void);
void fnSW_RealTime(void);
void fnSW_Clear(void);
U8 fnSW_GetState(U8 keynum);
void fnSW_Init(void)
{
    U8 i;
    for(i = 0; i< SW_SWITCH_NUM; i++)  //
    {
        SET_IO_IN_WITHOUTPULLUP(g_sw_stConfig[i].port, g_sw_stConfig[i].pin);
    }
    memset(&g_sw_stRegs, 0, sizeof(g_sw_stRegs));
    g_sw_stRegs.m_uiReadyTimer = 250;
}
U8 fnSW_GetState(U8 keynum)
{
    U8 uState=0;
    if(GET_IO_STA(g_sw_stConfig[keynum].port, g_sw_stConfig[keynum].pin) == g_sw_stConfig[keynum].level) {
        uState = 1;
    }
    return uState;
}


void fnSW_RealTime(void)
{
    U8 i, state;
    if(g_tm_stTimerFlag.Bits.bTimer8ms)
    {
        if(g_sw_stRegs.m_uiReadyTimer)
        {
            g_sw_stRegs.m_uiReadyTimer --;
        }
        for(i = 0; i< SW_SWITCH_NUM; i++)
        {
            state = fnSW_GetState(i);
            if(state != g_sw_stRegs.m_uchLevel[i])
            {
                g_sw_stRegs.m_uchCount[i] ++;
            }
            else
            {
                g_sw_stRegs.m_uchCount[i] = 0;
            }
            if(g_sw_stRegs.m_uchCount[i] > SW_DEBOUNCE_TIMES)
            {
                g_sw_stRegs.m_uchLevel[i] = state;
                g_sw_stRegs.m_uchTrigger[i] = TRUE;
                g_sw_stRegs.m_uchCount[i] = 0;
            }
        }
    }
}
void fnSW_Clear(void)
{
    U8 i;
    for(i = 0; i< SW_SWITCH_NUM; i++)
    {
        g_sw_stRegs.m_uchTrigger[i] = 0;
    }
}

void fnSW_ClearStart(void)
{
    uchStartLevel = FALSE;
}
U8 fnSW_IsReady(void) {
    U8 res = FALSE;
    if(0 == g_sw_stRegs.m_uiReadyTimer) {
        res = TRUE;
    }
    return(res);
}
