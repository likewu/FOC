//https://blog.csdn.net/aoyousihaiqiuqihuang/article/details/119980723
#include "pid2.h"

PIDdata_t PidCurrent = PI_CONTROLLER_DEFAULTS;

void updatePID( PIDdata_t *pid )
{
  float setpoint = pid->Ref;
  float measurement = pid->Fbk;
  /*Error signal*/
  float error = setpoint - measurement;
 
  /*Integral*/
  pid->integrator = pid->integrator +  pid->Ki*(error + pid->prevError);
  /* Anti-wind-up via integrator clamping */
  pid->integrator = sat(pid->integrator, pid->limMaxInt, pid->limMinInt);

  /* Derivative (band-limited differentiator)*/
  pid->differentiator = (error - pid->prevError); /* Note: derivative on measurement, therefore minus sign in front of equation! */
 
  /*Compute output and apply limits*/
  pid->Out = (pid->Kp) * error + (pid->Ki)* (pid->integrator) + (pid->Kd) * (pid->differentiator);
  pid->Out = sat(pid->Out, pid->limMax, pid->limMin);
 
  /* Store error and measurement for later use */
  pid->prevError       = error;
  pid->prevMeasurement = measurement;
}
