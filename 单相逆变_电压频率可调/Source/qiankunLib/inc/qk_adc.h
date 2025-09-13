/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_adc.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __QK_ADC_H
#define __QK_ADC_H

#include "power.h"

// 宏定义
#define ADC1_DR_Address ((u32)0x4001244C)
// ADC转换结果

// 函数声明
void AdcInit(void);

// 外部变量
extern u16 ADC_Results[4];

#endif
