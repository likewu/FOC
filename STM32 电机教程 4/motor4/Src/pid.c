#include "pid.h"



PIDdata_t PidCurrent = PI_CONTROLLER_DEFAULTS;




void updatePID( PIDdata_t *pid )
{
    
  /* proportional term */ 								
	pid->up = pid->Ref - pid->Fbk;										
																          
	/* integral term */ 										
	pid->ui = (pid->Out == pid->v1) ? (pid->Ki*pid->up + pid->i1) : pid->i1;
	pid->ui = sat(pid->ui, pid->Umax, pid->Umin);	
	pid->i1 = pid->ui;
																                    
	/* control output */ 										          
	pid->v1 = pid->Kp*(pid->up + pid->ui);							          
	pid->Out= sat(pid->v1, pid->Umax, pid->Umin);						      
	// pid->w1 = (pid->Out == pid->v1) ? 1.0 : 0.0;		
	// pid->i1= sat(pid->i1, pid->Umax/2,pid->Umin/2 );	  // 积分限幅，可以加
	
	
}