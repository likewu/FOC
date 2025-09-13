#ifndef _H_ADC_
#define _H_ADC_
/*
wx:1076279743
*/
#include "define.h"
#define AD_CHANNELNUM 2

enum
{
    AD_CHANNEL_OFF_VOLSET = 0, //AÏàµçÁ÷ A0 0
    AD_CHANNEL_OFF_U1,//B1 4-20mA
    AD_CHANNEL_NUM
};


typedef struct {
    GPIO_TypeDef *port;
    U8 pin;
} STRADPortConfig;

typedef struct
{
    U16 m_uiPM;
    U16 m_uiPN;
    INT16 m_iPK;
} STRCALParamType;

#define AD_SAMPLE_TIMES_PERPERIOD 50 //60->24ms, 50->20ms
typedef struct
{
    U16 m_uiCount[AD_CHANNEL_NUM];
    U32 m_ulADBuff[AD_CHANNEL_NUM];
    U32 m_ulADValue[AD_CHANNEL_NUM];
    U8 m_uchFinish[AD_CHANNEL_NUM];
    U16 m_uiADRes[AD_CHANNEL_NUM];
    U16 m_uiADFilterValue[AD_CHANNEL_NUM];
    U16 m_uiPWRFreq;
    U8 m_uchReady[AD_CHANNEL_NUM];
} ADRegsType;
extern ADRegsType g_ad_stRegs;

#define AD_GET_VAL(i) g_ad_stRegs.m_uiADRes[i]
extern void fnAD_Init(void);
extern void fnAD_RealTime(void);
extern void ADC_cal (void);
extern void fnAD_Clear(void);
extern void fnAD_DisableInt(void);
extern void fnAD_EnableInt(void);

#endif

