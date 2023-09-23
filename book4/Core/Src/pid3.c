//https://blog.csdn.net/aoyousihaiqiuqihuang/article/details/119980723
//https://github.com/pms67/PID
#include "pid3.h"

PIDdata_t PidCurrent = PI_CONTROLLER_DEFAULTS;

void updatePID( PIDdata_t *pid )
{
  float setpoint = pid->Ref;
  float measurement = pid->Fbk;
  /*Error signal*/
  float error = setpoint - measurement;

  /*Proportional*/
  float proportional = pid->Kp * error;
 
  /*Integral*/
  pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);
  /* Anti-wind-up via integrator clamping */
  pid->integrator = sat(pid->integrator, pid->limMaxInt, pid->limMinInt);

  /* Derivative (band-limited differentiator)*/
  /* Note: derivative on measurement, therefore minus sign in front of equation! */
  pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement) /* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);

  /*Compute output and apply limits*/
  pid->Out = proportional + pid->integrator + pid->differentiator;
  pid->Out = sat(pid->Out, pid->limMax, pid->limMin);
 
  /* Store error and measurement for later use */
  pid->prevError       = error;
  pid->prevMeasurement = measurement;
}
