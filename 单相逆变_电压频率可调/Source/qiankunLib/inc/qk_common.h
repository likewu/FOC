/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_common.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-11
Description		--

**************************************************************************/
#ifndef __QK_COMMON_H
#define __QK_COMMON_H

#include "power.h"

#define SYS_TASK_PERIOD_DEFAULT	(100)//Ĭ��ϵͳ�������� = 100 * 10us = 1ms

//��������õĶ�ʱ��(TIM7)
#define SYS_TASK_TIM			(TIM7)
#define RCC_APB1Periph_SYS_TASK_TIM	(RCC_APB1Periph_TIM7)
#define Task_Timer_Value		(SYS_TASK_TIM->CNT)			//��λ�� 10us
#define SetTaskPeriod(x)		(SYS_TASK_TIM->ARR=x-1) 	//��λ�� 10us,���ֵ65535
#define Task_Run				((SYS_TASK_TIM->SR & TIM_FLAG_Update)==0)
#define	ClearTaskFlag()			(SYS_TASK_TIM->SR = (u16)~TIM_FLAG_Update)
#define WaitTaskEnd()			{while(Task_Run);ClearTaskFlag();}

extern u32 System_Clock;//ϵͳʹ�õ�ʱ��Ƶ�ʣ�����RccInit()��ȷ��

void CloclkInit(void);
void DelayMs(u32 nms);
void DelayUs(u32 nus);


#endif

