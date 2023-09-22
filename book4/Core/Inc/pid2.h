#ifndef __PID2_H_
#define __PID2_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define sat(x, max, min)                            \																	                  \
			(x > max) ? max : ( (x < min) ?  min : x )


// PID Variables
typedef struct PIDdata
{
  float  Ref;       // Input: reference
  float  Fbk;       // Input: feedback

	/* Controller gains */
  float Kp;
  float Ki;
  float Kd;
  /* Output limits */
  float limMin;
  float limMax;
  /* Integrator limits */
  float limMinInt;
  float limMaxInt;
  /* Sample time (in seconds) */
  float T;
  /* Controller "memory" */
  float integrator;
  float prevError;      /* Required for integrator */
  float differentiator;
  float prevMeasurement;    /* Required for differentiator */
  /* Controller output */
  float Out;
} PIDdata_t;

#define PI_CONTROLLER_DEFAULTS {	\
						               0.0f, 		\
                           0.0f, 		\
						               0.9f, 		\
                           0.5f,	  \
                           0.0f,	  \
													 -10.0f,	  \
                           10.0f,	  \
                           -5.0f,  \
                           5.0f,	  \
                           0.1f, 	\
                           0.0f,	  \
                           0.0f,	  \
                           0.0f 	  \
                           0.0f     \
                           0.0f     \
              			  }
											
extern PIDdata_t PidCurrent;											
void updatePID( PIDdata_t *pid );

#endif
