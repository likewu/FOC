#ifndef __Bipolar_PWM_H
#define __Bipolar_PWM_H

#include "power.h"

extern s32 samp_number;

void Bipolar_pwm(u16 pwm_period,s32 samp_num);
void Bipolar_pwm1(u16 pwm_period,s32 samp_num,signed int cnt);
void sin_duty_init(u16 pwm_period,s32 samp_num);
void Vout_Ctr(u16 Cur_V,u16 Set_V,float Kp,float Ki);
	
#endif