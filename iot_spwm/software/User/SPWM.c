/*
 * File: SPWM.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "SPWM.h"
#include "IO.h"
#include "DMA.h"
#include "app.h"
#include "sindef.h"
#include "timer.h"
#define SPWM_PWM_FREQ 12000

#define SPWM_DIS_INT() NVIC_DisableIRQ(TIM1_UP_IRQn);
#define SPWM_EN_INT() NVIC_EnableIRQ(TIM1_UP_IRQn);

STRSPWMRegType g_spwm_stRegs;
STRSPWMCCRDataType g_spwm_stCCRBuff;
void fnSPWM_Update(U16 freq, U16 duty);
//maximum duty is (500us - 5us*2)=490, 4us is the dead time
//maximum duty is 490/500=98%, 9800
//一个周期内的输出占空比为62.5%,在该时间内输出半个周期的正弦波
//freq频率, alpha占空比, arp周期, ccrs占空比寄存器
U16 fnSPWM_CalAvg(U16 bi, U16 ei) {
    U16 i;
    U32 sumv;
    U16 tv;
    U16 rv;
    tv = 0;
    sumv = 0;
    rv = 0;
    for(i = bi; i < ei; i++) {
        if(i < SPWM_SIN_NSIZE) {
            sumv += spwm_sin_vals[i];
            tv ++;
        }
    }
    if(tv > 0) {
        sumv = sumv / tv;
        rv = (U16)sumv;
    }
    return(rv);
}
#define SPWM_TICK_PER_DEADZONE ((SPWM_PWM_CLOCK_FREQ / 1e6) *4) //4us deadtime
U16 fnSPWM_GetCCR(U16 freqconfig, U16 alpha, U16 arp, U16 *ccrs, U16 *totalbins) {
    U32 data;
    U16 bins, hbins, qbins, tbins;
    U16 ssn;
    U16 bi, ei, i;
    U16 ldata, avgv;
    float aval, bval;
    //输出PWM周期个数为1/freq*62.5%/(1/PWM)=PWM*62.5/(freq*100);
    U16 res = 0;
    *totalbins = 0;
    if(alpha > 9800) {
        alpha = 9800;
    }
    if(freqconfig > 3000) {
        return (res);
    }
    if(freqconfig == 0) {
        return(res);
    }
    data = (U32)(SPWM_PWM_FREQ * 10);
    data = (U32)data / freqconfig;
    if((data & 0x01) != 0) {
        data ++;
    }

    bins = (U16)data;
    if(g_spwm_stRegs.m_uchThreePhase == FALSE) {
        data = (U32)bins * 50;
    } else {
        data = (U32)bins * 63;
    }
    data = data / 100;
    hbins = (U16)data;

    qbins = hbins >> 1;
    tbins = qbins;
    if((hbins & 0x01)) {
        tbins ++;
    }
    ssn = SPWM_SIN_NSIZE / hbins;
    for(i = 0; i < tbins; i++) {
        bi = i * ssn;
        ei = i * ssn + ssn;
        avgv = fnSPWM_CalAvg(bi, ei);
        aval = (float)arp * (float)alpha;
        aval = (float)aval * (float)avgv;
        bval = (float)10000*(float)65535;

        aval = aval / bval;
        aval += (U32)SPWM_TICK_PER_DEADZONE;//deadzonetime
        if(aval > 65530) {
            aval = 65535;
        }
        ldata = (U16)aval;

        ccrs[i] = ldata;
    }
    for(i = 0; i < qbins; i++) {
        ccrs[tbins + i] = ccrs[qbins - i - 1];
    }
    for(i = hbins; i< bins; i++) {
        ccrs[i] = 0;
    }
    *totalbins = bins;
    res = hbins;
    return(res);
}

void fnSPWM_HWInit(void) {
    U16 arr;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;//Timer3 clock enable
    SPWM_PWM_TIMER->PSC = (SYSCLK_SYS_FREQ / SPWM_PWM_CLOCK_FREQ - 1);//fck_psc/(PSC[15:0] + 1)=16MHz
    SPWM_PWM_TIMER->DIER = TIM_DIER_UIE;
    arr = (SPWM_PWM_CLOCK_FREQ / SPWM_PWM_FREQ);//2K
#ifdef _CENTER_ALIGN_
    arr = arr / 2;//center-aligned pwm
    SPWM_PWM_TIMER->ARR = arr;
#else
    SPWM_PWM_TIMER->ARR = arr - 1;
#endif
    /*TDS = 72MHz, dead-time=31*1/72MHz=430ns, 4.03125us
    DTG[7:5]=0xx => DT=DTG[7:0]x tdtg with tdtg=tDTS.
    DTG[7:5]=10x => DT=(64+DTG[5:0])xtdtg with Tdtg=2xtDTS
    (64+x)*2/72MHz=4.03125us, 81
    DTG[7:5]=110 => DT=(32+DTG[4:0])xtdtg with Tdtg=8xtDTS
    (32+x)*8/72MHz=4.03125us=>x=4, 4us
    */

    SPWM_PWM_TIMER->CCMR1 = 0x6868;//TIM1_CH1
    SPWM_PWM_TIMER->CCMR2 = 0x0068;
#ifdef _CENTER_ALIGN_
    SPWM_PWM_TIMER->CCR1 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->CCR2 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->CCR3 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->CCER = 0x0555;//TIM1_CH1&TIM1_CH1N both are enabled, polarity active high
#else
    SPWM_PWM_TIMER->CCR1 = 0;
    SPWM_PWM_TIMER->CCR2 = 0;
    SPWM_PWM_TIMER->CCR3 = 0;
    SPWM_PWM_TIMER->CCER = 0x0FFF;//TIM1_CH1&TIM1_CH1N both are enabled, polarity active low
#endif
    SPWM_PWM_TIMER->BDTR = 0x80C4; //C4=4us=(32+4)*8/72, (32+31)*8/72=7us
    SPWM_PWM_TIMER->EGR = 0x0001;//Reload immediate


    SET_IO_LOW(P_PWM_A_PORT, P_PWM_A_PIN);
    SET_IO_LOW(P_PWM_AN_PORT, P_PWM_AN_PIN);

    SET_IO_LOW(P_PWM_B_PORT, P_PWM_B_PIN);
    SET_IO_LOW(P_PWM_BN_PORT, P_PWM_BN_PIN);

    SET_IO_LOW(P_PWM_C_PORT, P_PWM_C_PIN);
    SET_IO_LOW(P_PWM_CN_PORT, P_PWM_CN_PIN);

    NVIC_DisableIRQ(TIM1_UP_IRQn);
    SPWM_PWM_TIMER->CR1 = 0x0000;

}
void fnSPWM_Init(void) {
    memset(&g_spwm_stRegs, 0, sizeof(g_spwm_stRegs));

    g_spwm_stRegs.m_uchARR = (SPWM_PWM_CLOCK_FREQ / SPWM_PWM_FREQ);
    g_spwm_stRegs.m_uchFreq = 500;
    g_spwm_stRegs.m_uchDuty = 0;
    g_spwm_stRegs.m_uchThreePhase = FALSE;
    fnSPWM_Update(g_spwm_stRegs.m_uchFreq, g_spwm_stRegs.m_uchDuty);

    fnSPWM_HWInit();
}
void TIM1_UP_IRQHandler(void) {

    STRSPWMCCRDataType *ccrdata;
    g_spwm_stRegs.m_uchProDet = FALSE;
    if(g_spwm_stRegs.m_uchProDet) {
        SPWM_PWM_TIMER->CCR1 = SPWM_PWM_TIMER->ARR;
        SPWM_PWM_TIMER->CCR2 = SPWM_PWM_TIMER->ARR;
        SPWM_PWM_TIMER->CCR3 = SPWM_PWM_TIMER->ARR;
        SPWM_PWM_TIMER->EGR = 0x0001;
    } else {
        if(TIM1->SR & TIM_SR_UIF) {

            if(SPWM_PWM_TIMER->CR1 & 0x0010) {
                ccrdata = &g_spwm_stRegs.m_stCCRData[g_spwm_stRegs.m_uchCCRDataIndex];
                if(g_spwm_stRegs.m_uchCCRDataPointer >= ccrdata->m_uchNSize) { //switch to new
                    if(g_spwm_stRegs.m_uchCCRDataReq == TRUE) {
                        g_spwm_stRegs.m_uchCCRDataIndex = g_spwm_stRegs.m_uchCCRDataNewIndex;
                        ccrdata = &g_spwm_stRegs.m_stCCRData[g_spwm_stRegs.m_uchCCRDataIndex];
                        g_spwm_stRegs.m_uchCCRDataReq = FALSE;
                    }
                    g_spwm_stRegs.m_uchCCRDataPointer  = 0;
                }
                SPWM_PWM_TIMER->CCR1 = ccrdata->m_uchCCRBuffs[0][g_spwm_stRegs.m_uchCCRDataPointer];
                SPWM_PWM_TIMER->CCR2 = ccrdata->m_uchCCRBuffs[1][g_spwm_stRegs.m_uchCCRDataPointer];//4
                SPWM_PWM_TIMER->CCR3 = ccrdata->m_uchCCRBuffs[2][g_spwm_stRegs.m_uchCCRDataPointer];//3
                if((g_spwm_stRegs.m_uchCCRDataPointer == ccrdata->m_uchNSize / 2)
                        || (g_spwm_stRegs.m_uchCCRDataPointer == 0)) {
                    g_spwm_stRegs.m_uchFireDetEn = TRUE;
                }
                g_spwm_stRegs.m_uchCCRDataPointer++;
            }
        }
    }
    TIM1->SR = 0x0000;
}
void fnSPWM_Update(U16 freq, U16 duty) {
    U16 i, nbi, nci;
    U16 data;
    g_spwm_stRegs.m_uchFreq = freq;
    g_spwm_stRegs.m_uchDuty = duty;
    memset(&g_spwm_stCCRBuff, 0, sizeof(g_spwm_stCCRBuff));
    g_spwm_stCCRBuff.m_uchSinBin = fnSPWM_GetCCR(g_spwm_stRegs.m_uchFreq, g_spwm_stRegs.m_uchDuty, g_spwm_stRegs.m_uchARR, g_spwm_stCCRBuff.m_uchCCRBuffs[0], &g_spwm_stCCRBuff.m_uchNSize);
    if(g_spwm_stRegs.m_uchThreePhase == FALSE) {
        nbi = g_spwm_stCCRBuff.m_uchNSize / 2; //single phase 2,
        nci = nbi;
    } else {
        nbi = g_spwm_stCCRBuff.m_uchNSize / 3;
        nci = (g_spwm_stCCRBuff.m_uchNSize * 2);
        nci = nci / 3;
    }
    for(i = 0; i < g_spwm_stCCRBuff.m_uchNSize; i++) {
        if(nbi >= g_spwm_stCCRBuff.m_uchNSize) {
            nbi = 0;
        }
        if(nci >= g_spwm_stCCRBuff.m_uchNSize) {
            nci = 0;
        }
        data = g_spwm_stCCRBuff.m_uchCCRBuffs[0][i];
#ifdef _CENTER_ALIGN_
        data = data / 2;
        if(SPWM_PWM_TIMER->ARR > data) {
            data = SPWM_PWM_TIMER->ARR - data;
        } else {
            data = 0;
        }
        g_spwm_stCCRBuff.m_uchCCRBuffs[0][i] = data;
#endif

        g_spwm_stCCRBuff.m_uchCCRBuffs[1][nbi] = data;
        if(g_spwm_stRegs.m_uchThreePhase == FALSE) {
            g_spwm_stCCRBuff.m_uchCCRBuffs[2][nci] = 0;
        } else {
            g_spwm_stCCRBuff.m_uchCCRBuffs[2][nci] = data;
        }
        nbi ++;
        nci ++;
    }
    g_spwm_stRegs.m_uchUpdated = TRUE;

}
void fnSPWM_CopyCCRData(STRSPWMCCRDataType *dstdata, STRSPWMCCRDataType *srcdata) {
    U16 i;
    dstdata->m_uchNSize = srcdata->m_uchNSize;
    dstdata->m_uchSinBin = srcdata->m_uchSinBin;
    for(i = 0; i < dstdata->m_uchNSize; i++) {
        dstdata->m_uchCCRBuffs[0][i] = srcdata->m_uchCCRBuffs[0][i];
        dstdata->m_uchCCRBuffs[1][i] = srcdata->m_uchCCRBuffs[1][i];
        dstdata->m_uchCCRBuffs[2][i] = srcdata->m_uchCCRBuffs[2][i];
    }

}

void fnSPWM_FillCCRData(STRSPWMCCRDataType *ccrdata) {
    U16 nindex;

    SPWM_DIS_INT();
    nindex = 0;
    if(g_spwm_stRegs.m_uchCCRDataIndex == 0) {
        nindex = 1;
    }
    fnSPWM_CopyCCRData(&g_spwm_stRegs.m_stCCRData[nindex], ccrdata);
    g_spwm_stRegs.m_uchCCRDataNewIndex = nindex;
    g_spwm_stRegs.m_uchCCRDataReq = TRUE;
    SPWM_EN_INT();

}
//最大100%, 10000
void fnSPWM_UpdateDuty(U16 duty) {
    fnSPWM_Update(500, duty);
    fnSPWM_FillCCRData(&g_spwm_stCCRBuff);
}
//maxmum 5000
void fnSPWM_RealTime(void) {

}

void fnSPWM_DisableIPWM(void) {
    U16 data = 0;
    SPWM_DIS_INT();
#ifdef _CENTER_ALIGN_
    data = SPWM_PWM_TIMER->ARR - data;
#endif
    SET_IO_LOW(P_PWM_A_PORT, P_PWM_A_PIN);
    SET_IO_LOW(P_PWM_AN_PORT, P_PWM_AN_PIN);

    SET_IO_LOW(P_PWM_B_PORT, P_PWM_B_PIN);
    SET_IO_LOW(P_PWM_BN_PORT, P_PWM_BN_PIN);

    SET_IO_LOW(P_PWM_C_PORT, P_PWM_C_PIN);
    SET_IO_LOW(P_PWM_CN_PORT, P_PWM_CN_PIN);

    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC1E;
    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC2E;
    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC3E;
    SPWM_PWM_TIMER->CCR1 = data;
    SPWM_PWM_TIMER->CCR2 = data;
    SPWM_PWM_TIMER->CCR3 = data;
    SPWM_PWM_TIMER->EGR = 0x0001;
    SPWM_PWM_TIMER->CR1 = 0;

    memset(g_spwm_stRegs.m_stCCRData, 0, sizeof(g_spwm_stRegs.m_stCCRData));
    g_spwm_stRegs.m_uchCCRDataIndex = 0;
    g_spwm_stRegs.m_uchProDet = FALSE;

}

void fnSPWM_EnableIPWM(void) {
    U16 data = 0;
#ifdef _CENTER_ALIGN_
    data = SPWM_PWM_TIMER->ARR - data;
#endif
    SPWM_PWM_TIMER->CCR1 = data;
    SPWM_PWM_TIMER->CCR2 = data;
    SPWM_PWM_TIMER->CCR3 = data;

    SPWM_PWM_TIMER->CCER |= TIM_CCER_CC1E;
    SPWM_PWM_TIMER->CCER |= TIM_CCER_CC2E;
    SPWM_PWM_TIMER->CCER |= TIM_CCER_CC3E;

    SET_IO_AFMODE_PP(P_PWM_A_PORT, P_PWM_A_PIN);
    SET_IO_AFMODE_PP(P_PWM_AN_PORT, P_PWM_AN_PIN);

    SET_IO_AFMODE_PP(P_PWM_B_PORT, P_PWM_B_PIN);
    SET_IO_AFMODE_PP(P_PWM_BN_PORT, P_PWM_BN_PIN);

    SET_IO_AFMODE_PP(P_PWM_C_PORT, P_PWM_C_PIN);
    SET_IO_AFMODE_PP(P_PWM_CN_PORT, P_PWM_CN_PIN);


    SPWM_PWM_TIMER->EGR = 0x0001;
    fnSPWM_Update(500, 0);
    fnSPWM_CopyCCRData(&g_spwm_stRegs.m_stCCRData[0], &g_spwm_stCCRBuff);
    g_spwm_stRegs.m_uchCCRDataIndex = 0;
    g_spwm_stRegs.m_uchProDet = FALSE;
#ifdef _CENTER_ALIGN_
    SPWM_PWM_TIMER->CR1 = ((0x03 << 5) | 1);
#else
    SPWM_PWM_TIMER->CR1 = 1;
#endif

    SPWM_EN_INT();
}

void SPWM_SETOUTLOW() {
    P_PWM_A_PORT->BRR = ((U32)0x01<<P_PWM_A_PIN);
    P_PWM_AN_PORT->BRR = ((U32)0x01<<P_PWM_AN_PIN);
    P_PWM_B_PORT->BRR = ((U32)0x01<<P_PWM_B_PIN);
    P_PWM_BN_PORT->BRR = ((U32)0x01<<P_PWM_BN_PIN);
    P_PWM_C_PORT->BRR = ((U32)0x01<<P_PWM_C_PIN);
    P_PWM_CN_PORT->BRR = ((U32)0x01<<P_PWM_CN_PIN);
    SET_IO_OUT_ISR(P_PWM_A_PORT, P_PWM_A_PIN);
    SET_IO_OUT_ISR(P_PWM_AN_PORT, P_PWM_AN_PIN);
    SET_IO_OUT_ISR(P_PWM_B_PORT, P_PWM_B_PIN);
    SET_IO_OUT_ISR(P_PWM_BN_PORT, P_PWM_BN_PIN);
    SET_IO_OUT_ISR(P_PWM_C_PORT, P_PWM_C_PIN);
    SET_IO_OUT_ISR(P_PWM_CN_PORT, P_PWM_CN_PIN);
    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC1E;
    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC2E;
    SPWM_PWM_TIMER->CCER &= ~TIM_CCER_CC3E;
    SPWM_PWM_TIMER->CCR1 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->CCR2 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->CCR3 = SPWM_PWM_TIMER->ARR;
    SPWM_PWM_TIMER->EGR = 0x0001;
    SPWM_PWM_TIMER->CR1 = 0x0000;
}
