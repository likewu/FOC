#ifndef _H_SPWM_
#define _H_SPWM_

#include "define.h"
/*
wx:1076279743
*/

#define _CENTER_ALIGN_ (TRUE)
#define SPWM_PWM_TIMER TIM1
#define SPWM_PWM_CLOCK_FREQ 72000000
#define P_PWM_A_PORT GPIOA //TIM1_CH1
#define P_PWM_A_PIN 8

#define P_PWM_AN_PORT GPIOB //TIM1_CH1
#define P_PWM_AN_PIN 13

#define P_PWM_B_PORT GPIOA //TIM1_CH1
#define P_PWM_B_PIN 9

#define P_PWM_BN_PORT GPIOB //TIM1_CH1
#define P_PWM_BN_PIN 14

#define P_PWM_C_PORT GPIOA //TIM1_CH1
#define P_PWM_C_PIN 10

#define P_PWM_CN_PORT GPIOB //TIM1_CH1
#define P_PWM_CN_PIN 15


#define SPWM_CCR_BUFF_SIZE 512
#define SPWM_CCR_PHASE_SIZE 3



#define IG_IGBT_SETOUTLOW() {\
	fnIG_UpdateIGBTPWM(0);\
}
typedef struct {
    U16 m_uchCCRBuffs[SPWM_CCR_PHASE_SIZE][SPWM_CCR_BUFF_SIZE];
    U16 m_uchNSize;
    U16 m_uchSinBin;
} STRSPWMCCRDataType;
typedef struct {
    U16 m_uchFreq;
    U16 m_uchNsize;
    U16 m_uchSinBin;
    U8 m_uchUpdated;
    U16 m_uchARR;
    U16 m_uchDuty;

    U16 m_uchCCRDataIndex;
    U16 m_uchCCRDataPointer;
    U16 m_uchCCRDataNewIndex;
    U8 m_uchCCRDataReq;
    STRSPWMCCRDataType m_stCCRData[2];
    U8 m_uchThreePhase;
    U8 m_uchFireDetEn;
    U8 m_uchProDet;
} STRSPWMRegType;
extern STRSPWMRegType g_spwm_stRegs;
extern STRSPWMCCRDataType g_spwm_stCCRBuff;
extern U16 fnSPWM_GetCCR(U16 freq, U16 alpha, U16 arp, U16 *ccrs, U16 *totalbins);
extern void fnSPWM_HWInit(void);
extern void fnSPWM_RealTime(void);
extern void fnSPWM_Init(void);
extern void fnSPWM_ISR(void);
extern void fnSPWM_DisableIPWM(void);
extern void fnSPWM_UpdateDuty(U16 duty);
extern void fnSPWM_EnableIPWM(void);
extern void SPWM_SETOUTLOW(void);
#endif
