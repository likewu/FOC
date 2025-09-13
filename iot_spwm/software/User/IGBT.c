
#include "PWM.h"
#include "IO.h"
#include "app.h"
#include "keydatastore.h"
#include "timer.h"
U16 g_ig_igbtduty;
U16 g_ig_igbtfreq;
U8 g_ig_pwmon;

void fnIG_Init(void);
void fnIG_RealTime(void);

#define V1SYNC_PWM_CLOCK_FREQ 2000000
void fnIG_V1SyncPWM_Update(U16 f) {
    U32 data;
    U16 duty;
    data = 40000;

    if(f > 10) {
        data = (U32)20000000 / f;
    }
    if(data > 65535) {
        duty = 65535;
    }
    else {
        if(data  > 2000) {
            duty = (U32)data - 1;
        }
    }
    IG_V1SYNC_PWM_TIMER->ARR = duty;
    IG_V1SYNC_PWM_TIMER->CCR2 = (duty >> 1);
}
void fnIG_V1SyncPWM_En(U16 f)
{


    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;//Timer3 clock enable
    IG_V1SYNC_PWM_TIMER->PSC = (SYSCLK_SYS_FREQ / V1SYNC_PWM_CLOCK_FREQ - 1);//fck_psc/(PSC[15:0] + 1)=16MHz 4
    IG_V1SYNC_PWM_TIMER->DIER = 0x0001;
    fnIG_V1SyncPWM_Update(f);
    IG_V1SYNC_PWM_TIMER->BDTR = 0x8000; //0x8000-Main output enable
    IG_V1SYNC_PWM_TIMER->CCMR1 = 0x6800;//TIM1_CH1
    IG_V1SYNC_PWM_TIMER->CCMR2 = 0x0000;//TIM1_CH1N
    IG_V1SYNC_PWM_TIMER->CCER = 0x0090;//TIM1_CH1&TIM1_CH1N
    IG_V1SYNC_PWM_TIMER->EGR = 0x0001;//Reload immediate
    IG_V1SYNC_PWM_TIMER->CR1 = 0x0001 | TIM_CR1_ARPE;
    SET_IO_AFMODE_PP(P_PWM_V1DET_SYNC_PORT, P_PWM_V1DET_SYNC_PIN);

}
void fnIG_V1SyncPWM_Dis(void) {
    P_PWM_V1DET_SYNC_PORT->BRR = ((U32)0x01<<P_PWM_V1DET_SYNC_PIN);
    SET_IO_OUT(P_PWM_V1DET_SYNC_PORT, P_PWM_V1DET_SYNC_PIN);
    IG_V1SYNC_PWM_TIMER->CCER &= ~TIM_CCER_CC1E;
    IG_V1SYNC_PWM_TIMER->CCR1 = 0;
    IG_V1SYNC_PWM_TIMER->EGR = 0x0001;
    IG_V1SYNC_PWM_TIMER->CR1 = 0x0000;

}

void fnIG_OtherTimer_Init(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;//Timer3 clock enable

    IG_OTHER_PWM_TIMER->CR1 = TIM_CR1_ARPE;//
    // TIM4->ARR = BS_BIT_PERIOD - 1;//205*0.5=102.5us
    IG_OTHER_PWM_TIMER->PSC = (SYSCLK_SYS_FREQ / PWM_OTHER_CLOCK_FREQ - 1);//fck_psc/(PSC[15:0] + 1)=16MHz
    IG_OTHER_PWM_TIMER->CCER = 0x0000;
    IG_OTHER_PWM_TIMER->DIER = 0x0000;
    IG_OTHER_PWM_TIMER->ARR = (PWM_OTHER_ARR - 1);
    IG_OTHER_PWM_TIMER->BDTR = 0x8000;
    IG_OTHER_PWM_TIMER->CCR1 = 0;
    IG_OTHER_PWM_TIMER->CCR2 = 0;
    IG_OTHER_PWM_TIMER->CCR3 = 0;
    IG_OTHER_PWM_TIMER->CCR4 = 0;
    IG_OTHER_PWM_TIMER->CCMR1 = 0x6868;//TIM8_CH1 2,3,4
    IG_OTHER_PWM_TIMER->CCMR2 = 0x6868;//TIM8_CH1 2,3,4
    IG_OTHER_PWM_TIMER->CCER = 0x9999;//TIM8_CH1 2,3,4
    IG_OTHER_PWM_TIMER->EGR = 0x0001;//Reload immediate
    IG_OTHER_PWM_TIMER->CR1 = 0x0001 | TIM_CR1_ARPE;
}


void fnIG_HVS_SetPWM(U16 dutypercent)
{
    U32 data;
    data = (U32)PWM_OTHER_ARR * dutypercent;
    data = data / 10000;
    IG_OTHER_PWM_TIMER->CCR2 = (U16)data;

}
void fnIG_HVS_SetOutLow(void)
{
    IG_OTHER_PWM_TIMER->CCR2 = 0;
}

void fnIG_HCS_SetPWM(U16 dutypercent)
{
    U32 data;
    data = (U32)PWM_OTHER_ARR * dutypercent;
    data = data / 10000;
    IG_OTHER_PWM_TIMER->CCR1 = (U16)data;

}
void fnIG_HCS_SetOutLow(void)
{
    IG_OTHER_PWM_TIMER->CCR1 = 0;

}

void fnIG_I2_SetPWM(U16 dutypercent)
{
    U32 data;
    data = (U32)PWM_OTHER_ARR * dutypercent;
    data = data / 10000;
    IG_OTHER_PWM_TIMER->CCR4= (U16)data;

}
void fnIG_I2_SetOutLow(void)
{
    IG_OTHER_PWM_TIMER->CCR4 = 0;

}
void fnIG_V2_SetPWM(U16 dutypercent)
{
    U32 data;
    data = (U32)PWM_OTHER_ARR * dutypercent;
    data = data / 10000;
    IG_OTHER_PWM_TIMER->CCR3= (U16)data;

}
void fnIG_V2_SetOutLow(void)
{
    IG_OTHER_PWM_TIMER->CCR3 = 0;

}

void fnIG_Init(void)
{

    fnIG_HVS_SetOutLow();
    fnIG_HCS_SetOutLow();
    fnIG_V2_SetOutLow();
    fnIG_I2_SetOutLow();
    fnIG_OtherTimer_Init();

    fnIG_HVS_SetOutLow();
    fnIG_HCS_SetOutLow();

    fnIG_HCS_SetPWM(0);
    fnIG_HVS_SetPWM(0);
    fnIG_V2_SetOutLow();
    fnIG_I2_SetOutLow();

    SET_IO_AFMODE_PP(P_PWM_HCS_PORT, P_PWM_HCS_PIN);
    SET_IO_AFMODE_PP(P_PWM_HVS_PORT, P_PWM_HVS_PIN);
    SET_IO_AFMODE_PP(P_PWM_I2_PORT, P_PWM_I2_PIN);
    SET_IO_AFMODE_PP(P_PWM_V2_PORT, P_PWM_V2_PIN);


}

void fnIG_RealTime(void)
{

}


