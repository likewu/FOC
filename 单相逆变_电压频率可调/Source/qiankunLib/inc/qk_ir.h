/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ir.h
Author			qiankunTeam
Version			V1.0.0
Date			2015-4-5
Description		--

**************************************************************************/
#ifndef __QK_IR_H
#define __QK_IR_H

#include "power.h"

// 红外遥控器需要用到的定时器和LED
#define IR_TIM				TIM6
#define IR_LED_ON()			(LED8_ON())
#define IR_LED_OFF()		(LED8_OFF())

// 红外解码时相应时间长短的定义
#define IR_DATA_IS_START  	((IR_counter >= 46)&&(IR_counter <= 55))
#define IR_DATA_IS_L		((IR_counter >= 9)&&(IR_counter <= 13))
#define IR_DATA_IS_H		((IR_counter >= 20)&&(IR_counter <= 24))
#define IR_DATA_IS_RE		(IR_counter >= 800)
#define IR_DATA_IS_HALF		(((IR_counter >= 0)&&(IR_counter <= 7))||(IR_counter >= 14)&&(IR_counter <= 17)||(IR_counter >= 42)&&(IR_counter <= 45))

extern u8 IR_Key;

//函数声明
void IrInit(void);
void IrDecode(void);

#endif