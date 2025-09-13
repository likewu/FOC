/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		power.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-1
Description		--

**************************************************************************/
#ifndef __POWER_H
#define __POWER_H

#include "stm32f10x_lib.h"
#include "qk_common.h"
#include "qk_gpio.h"
#include "qk_lcd.h"
#include "font.h"
#include "qk_pwm.h"
#include "sample.h"
#include "qk_ir.h"
#include "qk_adc.h"
#include "qk_menu.h"
#include "pic.h"
#include "Bipolar_PWM.h"

//#define Get_U1(adc_value)	(adc_value*5000/adc_Ref*k_U1/1000)
//#define Get_U2(adc_value)	(adc_value*5000/adc_Ref*k_U2/1000)
//#define Get_I1(adc_value)	((adc_value*5000/adc_Ref-b_I1)*(-k_I1)/10000)
//#define Get_I2(adc_value)	((adc_value*5000/adc_Ref-b_I2)*k_I2/10000)
//#define Get_U1_ADC(value)	(value*1000/k_U1*adc_Ref/5000)
//#define Get_U2_ADC(value)	(value*1000/k_U2*adc_Ref/5000)
//#define Get_I1_ADC(value)	((value*10000/(-k_I1)+b_I1)*adc_Ref/5000)
//#define Get_I2_ADC(value)	((value*10000/k_I2+b_I2)*adc_Ref/5000)
#define Rs	(221)	// 采样电阻(0.221)
#define Get_U1(adc_u1_value,adc_i1_value)	(adc_u1_value*k_U1/1000-Get_I1(adc_i1_value)*Rs/10000)
#define Get_U2(adc_value)					(adc_value*k_U2/1000)
#define Get_I1(adc_value)					(((adc_value)*(k_I1)/1000))
#define Get_U2_ADC(value)					(value*1000/k_U2)
#define Get_I1_ADC(value)					((value*1000/(k_I1)))

#define DUTY_MAX	(Period*98/100)
#define DUTY_MIN	(Period*10/100)

#define PWM_STOP()	{GPIOA->BRR =  GPIO_Pin_1;PwmFlag=0;}
#define PWM_START()	{GPIOA->BSRR =  GPIO_Pin_1;PwmFlag=1;}

#define RS_ON()		(GPIOB->BSRR =  GPIO_Pin_6)
#define RS_OFF()	(GPIOB->BRR =  GPIO_Pin_6)

#define OFFLINE()	(GPIOA->BSRR =  GPIO_Pin_3)
#define ONLINE()	(GPIOA->BRR =  GPIO_Pin_3)

extern s32 Duty,DutyRatio;
extern s32 I1,U1,I2,U2,P1,P2,Ef1,Ef2,I1_Set,U1_OF_Set,U1_UF_Set,I2_Set,U2_Set;
extern s32 Et,Flag;
extern s32 adc_I1,adc_I2,adc_U1,adc_U2,adc_Ref,adc_I1_Set,adc_U1_OF_Set,adc_U1_UF_Set,adc_I2_Set,adc_U2_Set;
extern s32 k_U1,k_U2,k_I1,k_I2;
extern s32 b_I1,b_I2;
extern s32 Mode,ChargeMode,DisChargeMode,DebugMode;
extern s32 adc_I1_min,adc_I1_max,adc_U1_min,adc_U1_max;
extern s32 adc_I2_min,adc_I2_max,adc_U2_min,adc_U2_max;
extern s32 PwmFlag,TaskWaitFlag;


/*******************/

extern s32 adc_umax;
extern s32 adc_umin;
extern s32 Amp;
extern s32 samp_number;
extern s32 set_frequency;
extern s32 tim5_period;
extern s32 adc_ch0;
extern s32 adc_ch1;


void Ctrl(void);
u16 MidFilter(u16 d1,u16 d2,u16 d3);
s32 adc_ref_mean(s32 di);
void Config();

// 以下是临时添加的灯
#define LED8_ON()	(GPIOF->BSRR =  GPIO_Pin_7)
#define LED8_OFF()	(GPIOF->BRR =  GPIO_Pin_7)


#endif 