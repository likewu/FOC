/*
 * File: ADC.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "ADC.h"
#include "DMA.h"
#include "IO.h"
#include "App.h"
#include "spwm.h"
ADRegsType g_ad_stRegs;



#define P_AD_VOLSET_PORT GPIOA
#define P_AD_VOLSET_PIN 4

#define P_AD_U1_PORT GPIOA
#define P_AD_U1_PIN 5


const STRADPortConfig ad_port_config[AD_CHANNELNUM] =
{
    {P_AD_VOLSET_PORT, P_AD_VOLSET_PIN}, //0
    {P_AD_U1_PORT, P_AD_U1_PIN }
};
const U8 adc_channels[AD_CHANNELNUM] = {4, 5};
void fnAD_Init(void);
void fnAD_RealTime(void);


#define ADC_DMA_BUFF_SIZE AD_CHANNELNUM * 2
U16 adc_dma_buff[ADC_DMA_BUFF_SIZE];

#define ADC_TIMER TIM4

void fnADC_PWRFreq(U16 pwrfreq) {
    U64 uwwData;
    U32 uwData;
    if(pwrfreq < 500) {
        pwrfreq = 500;
    }
    if(pwrfreq > 3000) {
        pwrfreq = 3000;
    }
    if(pwrfreq != g_ad_stRegs.m_uiPWRFreq) {
        uwData = AD_SAMPLE_TIMES_PERPERIOD * (U32)pwrfreq; //50
        uwwData = 12*1e7;

        uwwData = (uwwData)/uwData;
        g_ad_stRegs.m_uiPWRFreq = pwrfreq;
        ADC_TIMER->ARR = (U16)uwwData - 1;//50 samples per pwm, 625*0.25 //128*625*0.25=20ms --4MHz
        ADC_TIMER->CCR4 = (U16)(uwwData >> 1);
    }
}
void fnADC_TimerInit(U16 pwrfreq)
{
    g_ad_stRegs.m_uiPWRFreq = 0;


    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;//Timer2 clock enable
    ADC_TIMER->CR1 = TIM_CR1_ARPE;//自动加载
    // TIM4->ARR = BS_BIT_PERIOD - 1;//205*0.25=102.5us
    ADC_TIMER->PSC = (SYSCLK_SYS_FREQ /12000000  - 1);//fck_psc/(PSC[15:0] + 1)=12MHz
    ADC_TIMER->EGR = 0x0001;//Reload immediate
    fnADC_PWRFreq(pwrfreq);
    ADC_TIMER->EGR = 0x0001;//Reload immediate
    ADC_TIMER->CCMR2 = 0x6800;//使能PWM功能，并开启自动加载CH3

    ADC_TIMER->CCER= 0x9000;//
    ADC_TIMER->CR1 |= 0x0001;

}

void fnADC_InitDMA(void) {

    U32 tmpreg;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN);
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
    DMA1_Channel1->CCR = 0;
    DMA1_Channel1->CNDTR = 0;
    DMA1_Channel1->CPAR = 0;
    DMA1_Channel1->CMAR = 0;
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
    tmpreg = DMA1_Channel1->CCR;
    tmpreg &= DMA_CCR_CLEAR_Mask;
    tmpreg |= (DMA_TCIE_ENABLE | DMA_HFIE_ENABLE | DMA_DIR_PeripheralSRC | DMA_Mode_Circular
               | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable
               | DMA_PeripheralDataSize_HalfWord | DMA_MemoryDataSize_HalfWord
               | DMA_Priority_High | DMA_M2M_Disable);
    DMA1_Channel1->CCR = tmpreg;
    DMA1_Channel1->CNDTR = (ADC_DMA_BUFF_SIZE);
    DMA1_Channel1->CPAR = (U32)&ADC1->DR;
    DMA1_Channel1->CMAR = (U32)&adc_dma_buff;
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

#define ADC_SAMPLE_CONFIG_BITS 1
#define ADC_SQR_BITS_NUM 5


void fnADC_Config(void) {
    U8 i;
    U32 smpr2, smpr1;
    U32 sqr1, sqr2, sqr3;
    sqr1 =  ((AD_CHANNELNUM - 1) << 4 * ADC_SQR_BITS_NUM);
    sqr2 = 0;
    sqr3 = 0;
    smpr2 = 0;
    smpr1 = 0;
    for(i = 0; i < AD_CHANNELNUM; i++) {
        if(i < 6) {
            sqr3 |= adc_channels[i] << (i * ADC_SQR_BITS_NUM);

        } else if(i < 12) {
            sqr2 |= adc_channels[i] << ((i - 6) * ADC_SQR_BITS_NUM);
        } else {
            sqr1 |= adc_channels[i] << ((i - 12) * ADC_SQR_BITS_NUM);
        }
        if(adc_channels[i] < 10) {
            smpr2 |= ADC_SAMPLE_CONFIG_BITS << (adc_channels[i] * 3);
        } else {
            smpr1 |= ADC_SAMPLE_CONFIG_BITS <<((adc_channels[i] - 10) * 3);
        }
    }
    ADC1->SQR1 = sqr1;
    ADC1->SQR2 = sqr2;
    ADC1->SQR3 = sqr3;
    ADC1->SMPR2 = smpr2;
    ADC1->SMPR1 = smpr1;
}

void fnADC_InitLow(void)
{
    U32 value;
    U8 i;

    ADC1->CR2 &= ~ADC_CR2_ADON;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    for(i = 0; i< AD_CHANNELNUM; i++)
    {
        SET_IO_ANALOG(ad_port_config[i].port, ad_port_config[i].pin);
    }
    value = ADC1->CR1;
    value &= ~ADC_CR1_DUALMOD;//independent mode
    value |= ADC_CR1_SCAN;
    ADC1->CR1 = value;

    value = ADC1->CR2;
    value &= ~ADC_CR2_ALIGN;//右对齐
    value &= ~ADC_CR2_EXTSEL;
    value |= (ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_2); //Timer 4
    value &= ~ADC_CR2_CONT;
    value |= (ADC_CR2_EXTTRIG | ADC_CR2_DMA | ADC_CR2_SWSTART);
    ADC1->CR2 = value;


    fnADC_Config();

    ADC1->CR2 |= ADC_CR2_ADON;


    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while(ADC1->CR2 & ADC_CR2_RSTCAL);

    ADC1->CR2 |= ADC_CR2_CAL;
    while(ADC1->CR2 & ADC_CR2_CAL);


}


void fnADC_NVIC_Init(void)
{
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void fnAD_Init(void)
{
    //  extern void DSP28x_usDelay(Uint32 Count);
    memset(&g_ad_stRegs, 0, sizeof(g_ad_stRegs));
    fnADC_NVIC_Init();
    fnADC_InitDMA();
    fnADC_InitLow();

    fnADC_TimerInit(500);
}


void fnAD_RealTime(void)
{
    U8 i;
    U16 pm, pn;
    short pk;
    signed long wDataA;
    U32 uwData;
    U16 temp;
    U16 res;
    fnADC_PWRFreq(500);
    for(i = 0; i < AD_CHANNEL_NUM; i++)
    {
        if(g_ad_stRegs.m_uchFinish[i]) {
            g_ad_stRegs.m_uchFinish[i] = FALSE;
            g_ad_stRegs.m_uiADFilterValue[i] = g_ad_stRegs.m_ulADValue[i];
            g_ad_stRegs.m_uchReady[i] = TRUE;
        }
    }
}

#define AD_SAMPLE_TIMES_PERPERIOD 50
#define AD_U1_MEAN_AD 2208//(1.8/3.3*4096)
void DMA1_Channel1_IRQHandler(void)
{
    U8 i;
    U16 filtercount = 0;

    volatile U16 *advalue;
    U16 addata;
    if(DMA1->ISR & DMA_ISR_TCIF1){
		advalue = &adc_dma_buff[AD_CHANNEL_NUM];
	}else{
		advalue = &adc_dma_buff[0];
	}
    DMA1->IFCR = 0x00000007; // channel 1 HF

    for(i = 0; i < AD_CHANNEL_NUM; i++)
    {
        addata = advalue[i];
        if(AD_CHANNEL_OFF_U1 == i) {
            if(addata > AD_U1_MEAN_AD) {
                addata = addata - AD_U1_MEAN_AD;
            } else {
                addata = AD_U1_MEAN_AD - addata;
            }
        }
        g_ad_stRegs.m_uiADRes[i] = addata;

        g_ad_stRegs.m_ulADBuff[i] += addata; //1024,12位A/D左对齐
        g_ad_stRegs.m_uiCount[i] ++;

        filtercount = AD_SAMPLE_TIMES_PERPERIOD;

        if(g_ad_stRegs.m_uiCount[i] >= filtercount)
        {
            if(FALSE == g_ad_stRegs.m_uchFinish[i])
            {
                g_ad_stRegs.m_ulADValue[i] = (U32)g_ad_stRegs.m_ulADBuff[i] / filtercount;
                g_ad_stRegs.m_uchFinish[i] = TRUE;
            }
            g_ad_stRegs.m_ulADBuff[i] = 0;
            g_ad_stRegs.m_uiCount[i] = 0;
        }
    }
}

void fnAD_Clear(void)
{
}
void fnAD_DisableInt(void)
{

    NVIC_DisableIRQ(DMA1_Channel1_IRQn);
}
void fnAD_EnableInt(void)
{
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
