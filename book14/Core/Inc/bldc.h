#ifndef __BLDC_H_
#define __BLDC_H_



/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


typedef struct
{
	uint8_t EN;     		// bldc使能
	uint8_t Dir;     		// bldc方向
	uint8_t Hall;   		// bldc HALL 状态
	int16_t PwmRef;  	  // bldc开环速度参考
	
	int32_t SpeedRef;  	// bldc闭环速度
	int32_t SpeedBck;  	// bldc速度反馈
	uint8_t Status; 		// bldc状态
	
	int16_t Current;    // bldc电流
	
	uint32_t HallSpanTime;  // 两次Hall间隔时间
	uint8_t HallGet; 		    // 是否是hall触发中断

}BldcMotor_t;


extern BldcMotor_t mymotor;

extern TIM_HandleTypeDef htim2;


// 用于得到HALL传感器位置
uint8_t GetBldcHall(void);

//  电机驱动
void BldcStart( BldcMotor_t *motor,int16_t speed );
void BldcStop( BldcMotor_t *motor );
void BldcMove( BldcMotor_t *motor );
void BldcUpdataPwm( int16_t pwm );


int32_t GetBldcSpeed( BldcMotor_t *bldc, uint16_t tus );


#endif
