/**
  ******************************************************************************
  * @file    mc_position.c
  * @author  Ken An
  * @brief   This file provides firmware functions for position control
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mc_position.h"


/* Define and initial positi0n structure */
Position_Handle_t Position_M1 =
{
  .Position_Gain         = 1,
	.Postiion_Div          = 2048,
	
	/* Max 1000ms/(3000RPM*10/60) = 2*/
	.Speed_ACC_Gain        = 2,
	.Speed_ACC_Div         = 1,
	
	.Speed_ACC_Count       = 0,
	.Encoder_Update_Count  = 0,
	.Encoder_Direction     = 0,
	.Encoder_Pre_Direction = 0,
	.Mode_Flag             = P_SPEED_MODE,
	.Torque_First_Flag     = 0,
};

/**
  * @brief  PI / PID position parameters Motor 1
  */
#define PID_ANGLE_KP_DEFAULT 3000
#define AG_KPDIV 2048
#define AG_KPDIV_LOG 11

#define PID_ANGLE_KI_DEFAULT 2
#define AG_KIDIV 16384
#define AG_KIDIV_LOG 14

#define PID_ANGLE_KD_DEFAULT 0
#define AG_KDDIV 16384
#define AG_KDDIV_LOG 14

/* Define Position Iq position max = 30% IQMAX, in this demo IQMAX = 16000*/
#define IQ_POSITION 4000

PID_Handle_t PIDAngleHandle_M1 =
{
  .hDefKpGain          = (int16_t)PID_ANGLE_KP_DEFAULT,
  .hDefKiGain          = (int16_t)PID_ANGLE_KI_DEFAULT, 
  .wUpperIntegralLimit = (int32_t)IQ_POSITION * (int32_t)AG_KIDIV,
  .wLowerIntegralLimit = -(int32_t)IQ_POSITION * (int32_t)AG_KIDIV,
  .hUpperOutputLimit       = (int16_t)IQ_POSITION, 
  .hLowerOutputLimit       = -(int16_t)IQ_POSITION,
  .hKpDivisor          = (uint16_t)AG_KPDIV,
  .hKiDivisor          = (uint16_t)AG_KIDIV,
  .hKpDivisorPOW2      = (uint16_t)AG_KPDIV_LOG,
  .hKiDivisorPOW2      = (uint16_t)AG_KIDIV_LOG,
  .hDefKdGain           = (int16_t)PID_ANGLE_KD_DEFAULT,
  .hKdDivisor           = (uint16_t)AG_KDDIV,
  .hKdDivisorPOW2       = (uint16_t)AG_KDDIV_LOG,
};

/**
  * @brief  It return absolute value of input
  * @param  dat: input data
  * @retval absolute value
  */
uint32_t Abs_Value(int32_t dat)
{
	uint32_t temp;
	if(dat < 0)
	{
		temp = -dat;
	}
	else
	{
		temp = dat;
	}
	return temp;
}

/**
  * @brief  Return error angle unit in rad*10000
  * @param  angle_ref: Target angle
  * @retval error angle
  */
int32_t Position_GetErrorAngle(Abs_Encoder_Handle_t * pHandle, int32_t angle_ref)
{
	int32_t wAngle;
	
  Position_M1.hTargetAngle = (int32_t)angle_ref;
	
	/* Measure angle = count*2PI*10000 + Mec_Angle*2PI*10000/65536 
	*/
  wAngle = (int32_t)(pHandle->Circle_Counter)*PI_MUL + (int32_t)(pHandle->Encoder_MecAngle)*PI_MUL/65536;
	
	Position_M1.hMeasuredAngle = wAngle;	
	
	/* Compute angle error */
  Position_M1.hError_Angle = Position_M1.hTargetAngle - Position_M1.hMeasuredAngle;
	
	/* If enter target range of position, then change to Torque mode */
	if(Abs_Value(Position_M1.hError_Angle) < CHANGE_LIMIT_LOW)
	{
		Position_M1.Mode_Flag = P_TORQUE_MODE;
	}
	/* Else if motor run in speed mode */
	else if(Abs_Value(Position_M1.hError_Angle) > CHANGE_LIMIT_HIGH)
	{
		Position_M1.Mode_Flag = P_SPEED_MODE;
		Position_M1.Torque_First_Flag = 0;
	}
	
	return  Position_M1.hError_Angle;
}

/**
  * @brief  It computes the new values for max speed
  * @param  None
  * @retval Speed referrence
  */
int16_t Position_CalcSpeedReferrence(void)
{
	int32_t hSpeedReference = 0;
	
	hSpeedReference = Position_M1.hError_Angle * Position_M1.Position_Gain / Position_M1.Postiion_Div;
	
	/* Limit max referrence speed*/
	if(hSpeedReference > MOTOR_MAX_SPEED_RPM)
	{
		hSpeedReference = MOTOR_MAX_SPEED_RPM;
	}
	else if(hSpeedReference < -MOTOR_MAX_SPEED_RPM)
	{
		hSpeedReference = -MOTOR_MAX_SPEED_RPM;
	}

  return (int16_t)hSpeedReference;
}

/**
  * @brief  It computes the new values current referrence
  * @param  target angle
  * @retval Iqref
  */
int16_t Position_CalcTorqueReferrence(void)
{
  int16_t hTorqueReference = 0;

  /* Calculate Iqref using PID*/
  hTorqueReference = PI_Controller( &PIDAngleHandle_M1, ( int32_t )Position_M1.hError_Angle );

  return hTorqueReference;
}



/******END OF FILE****/
