#ifndef __STEPMOTOR_H_
#define __STEPMOTOR_H_




/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


typedef struct
{
	uint8_t EN;     // 步进电机使能
	uint8_t Step;   // 步进电机当前的节拍
	uint8_t Dir;    // 步进电机方向
	uint8_t Status; // 步进电机状态
	__IO uint32_t *Delay; // 步进电机节拍延时，单位US
}stepmotor_t;


//extern stepmotor_t mymotor;

void setpwm(uint16_t pwm);
void step4(uint8_t step);
void step8(uint8_t step);


#endif
