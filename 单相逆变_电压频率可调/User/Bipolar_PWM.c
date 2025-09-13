#include "Bipolar_PWM.h"
#include "math.h"

s32 Amp=90;
s32 set_frequency=31;
signed short sin_data[5000];
s32 samp_number=360;


/**************************************

function:产生以pwm定时器周期为峰值的正弦信号
timer_period:产生pwm的定时器周期
number:将2pi分成number个点

****************************************/
void sin_duty_init(u16 timer_period,s32 samp_num)
{
	float Omega=0; 
	u16 i;
	for(i=0;i<samp_num;i++)
	{
		Omega=2.0*3.1415926*(float)i/(float)samp_num;
		sin_data[i]=timer_period*0.5*sin(Omega);
		//sin_data[i]=timer_period*0.5*((float)(Amp)/100.0*sin(Omega)+1);
	}	
}
/*************************************

function:产生以pwm定时器周期为峰值的正弦信号
timer_period:产生pwm的定时器周期
number:将2pi分成number个点

**************************************/

void Bipolar_pwm(u16 pwm_period,s32 samp_num)
{
	static s32 sin_duty;
	static u16 i=0;
	if(i > samp_num - 1)
	{
		i = 0;
	}
	sin_duty = sin_data[i]*(float)(Amp)/100.0+(pwm_period>>1);
	//sin_duty=sin_data[i];
	if(sin_duty < 2)
	{
		sin_duty = 2;
	}
	if(sin_duty > pwm_period - 2)
	{
		sin_duty = pwm_period - 2;
	}
	TIM8->CCR1 = sin_duty;
	i+=1;
}

//void Bipolar_pwm1(u16 pwm_period,s32 samp_num,signed int cnt)
//{
//	static u16 sin_duty;
//	static u16 ii=0;
//	if(ii > samp_num - 1)
//	{
//		ii = 0;
//	}
//	if(cnt==0)//奇数次进中断计算，偶数次进中断给pwm
//	{
//		
//		sin_duty = sin_data[ii]*(float)(Amp)/100.0+(pwm_period>>1);
//		ii++;
//		
//	}
//	else
//	{
//		if(sin_duty < 2)
//		{
//			sin_duty = 2;
//		}
//		if(sin_duty > pwm_period - 2)
//		{
//			sin_duty = pwm_period - 2;
//		}
//		TIM8->CCR1 = sin_duty;
//	}
//	
//	
//}

signed long PI_Calculate(u16 Cur_value,u16 Set_value,float Kp,float Ki)
{
	signed int PID_result;
	register signed int Error=0;
	static u32 iError=0;
	Error=Set_value-Cur_value;
	iError+=Error;
	if(iError>3000)iError=3000;
	if(iError<3000)iError=-3000;
	
//	printf("Set_value:%d\n",Set_value);
//	printf("Cur_value:%d\n",Cur_value);
//	printf("Error:%d\n",Error);
//	printf("iError:%d\n",iError);
	
  return (Kp*Error+Ki*iError)/1000;
}

void Vout_Ctr(u16 Cur_V,u16 Set_V,float Kp,float Ki)
{
	float A=0;
	A=PI_Calculate(Cur_V,Set_V,Kp,Ki);
//	printf("A:%f\n",A);
	if(A<0)A=0;
	if(A>1)A=1;
	Amp+=A;
	
//  printf("Amp:%f\n",Amp);
}
