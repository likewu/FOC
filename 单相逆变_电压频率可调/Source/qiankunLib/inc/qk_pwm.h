/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_pwm.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-9
Description		--

**************************************************************************/
#ifndef __QK_PWM_H
#define __QK_PWM_H

#include "power.h"
//宏定义
#define 	PWM_CLK						(System_Clock)
#define 	DeadTime					(0)
#define 	PERIOD					2240	//(PWM_CLK/2000000*(20))
#define		Period						PERIOD

//函数声明
void PwmInit(void);
void TIM8_Init();
void UpdateDutyA(s32 dutyA);
void UpdateDutyB(s32 dutyB);
void UpdateDutyC(s32 dutyC);
void TIM5_Config(unsigned short int period,unsigned short int Prescaler);
#endif