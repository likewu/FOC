#ifndef __PID_H_
#define __PID_H_




/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"



#define sat(x, max, min)                            \
																	                  \
			(x > max) ? max : ( (x < min) ?  min : x )    \
			
			

// PID Variables
typedef struct PIDdata
{
		float  Ref;   		// Input: reference
		float  Fbk;   		// Input: feedback
		float  Out;   		// Output: controller output 
		float  Kp;				// Parameter: proportional loop gain
		float  Ki;			  // Parameter: integral gain
		float  Kd;			  // Parameter: integral gain
		float  Umax;			// Parameter: upper saturation limit
		float  Umin;			// Parameter: lower saturation limit
		float  up;				// Data: proportional term
		float  ui;				// Data: integral term
		float  v1;				// Data: pre-saturated controller output
		float  i1;				// Data: integrator storage: ui(k-1)
		float  w1;				// Data: saturation record: [u(k-1) - v(k-1)]
} PIDdata_t;


#define PI_CONTROLLER_DEFAULTS {	\
						               0, 		\
                           0, 		\
						               0, 		\
                           1.0,	  \
                           0.0,	  \
													 0.0,	  \
                           1.0,	  \
                           -1.0,  \
                           0.0,	  \
                           0.0, 	\
                           0.0,	  \
                           0.0,	  \
                           1.0 	  \
              			  }

											
											
extern PIDdata_t PidCurrent;											
void updatePID( PIDdata_t *pid );



#endif
