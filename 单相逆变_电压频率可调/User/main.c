/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		main.c
Author			qiankunTeam
Version			V1.0.0
Date			2015-4-16
Description		PWM_test

**************************************************************************/

#include "power.h"

#define CHAR_COLOR	RED
#define BACK_COLOR	CYAN

s32 Duty,DutyRatio;
// 2位小数
s32 I1,U1,I2,U2,P1,P2,Ef1,Ef2,I1_Set,U1_OF_Set,U1_UF_Set,I2_Set,U2_Set;
s32 adc_I1,adc_I2,adc_U1,adc_U2,adc_Ref,adc_I1_Set,adc_U1_OF_Set,adc_U1_UF_Set,adc_I2_Set,adc_U2_Set;
s32 Et,Flag;
s32 adc_I1_min,adc_I1_max,adc_U1_min,adc_U1_max;
s32 adc_I2_min,adc_I2_max,adc_U2_min,adc_U2_max;
// 5位小数
s32 k_U1,k_U2,k_I1,k_I2;
s32 b_I1,b_I2;
s32 ChargeMode=0,DisChargeMode=0,DebugMode=0;
s32 PwmFlag,TaskWaitFlag;

void Config();
void DisplayPanel(void);
void Welcome(void);
void Bsp_init(void);
int main()
{ 
	s32 cnt=0;
	u8 led_data;
	s8 led_n=0,led_dir=0;
	k_U1=1063;
	k_U2=1082;
	k_I1=593;
	
	Bsp_init();
	while(1)
	{
		if(IR_Key==3)
		{
			LCD_Clear(BACK_COLOR);
			LCD_DrawPic(0, 0, 319,239,0,0,gImage_standby);
			SetParameter();
			DisplayPanel();
		}
		IR_Key=0;
		TaskWaitFlag=0;
		WaitTaskEnd();
	}		
 
}
 
void Bsp_init()
{
	// 时钟的配置
	CloclkInit();
	DelayMs(50);
	LCD_Init();
	LCD_SetBright(100);
	LCD_Clear(BACK_COLOR);
	LCD_SelectFont(&FONT_Default);
	IrInit();
	
	sin_duty_init(PERIOD,samp_number);
	TIM8_Init();
	TIM5_Config((u16)(112000000/(float)(samp_number*set_frequency)-1),0);
	
	AdcInit();
	GpioInit();
	InitParameter();
	DelayMs(100);
	Config();
	I1_Set=10;
	Welcome();
	DisplayPanel();
	
}



void Config()
{
	adc_I1_min=Get_I1_ADC((s32)10);
	adc_I1_max=Get_I1_ADC((s32)210);
	adc_U2_min=Get_U2_ADC((s32)800);
	adc_U2_max=Get_U2_ADC((s32)4000);
}

void DisplayPanel(void)
{
	LCD_Clear(BACK_COLOR);
	LCD_DrawPic(0, 0, 319,239,0,0,gImage_standby);
}	
void Welcome(void)
{
	LCD_DrawPic(0, 0, 319,239,0,0,gImage_welcome);
	DelayMs(3000);
	LCD_Clear(BACK_COLOR);
}
/*
void Ctrl(void)
{
	s32 et,u1;
	static s32 k;
	static s32 duty;
	static s32 over_charge_cnt,over_discharge_cnt;
	s32 duty_real;
	if(TaskWaitFlag) return;
	duty_real=adc_U1*Period/adc_U2;
	if(duty_real>Period) duty_real=Period;
	else if(duty_real<0) duty_real=0;
	if(ChargeMode)
	{
		RS_ON();
		ONLINE();
//		if(adc_I1_Set<adc_I1_min)
//		{
//			PWM_STOP();
//		}
//		else
		{
			u1=Get_U1(adc_U1,adc_I1);
			if(u1>U1_OF_Set) // 过充保护
			{
				over_charge_cnt++;
				if(over_charge_cnt >= 5)// 连续5次过充就关闭
				{
					// 停止充电
					ChargeMode=0;
					OFFLINE();
					duty=0;
					Duty=0;
					PWM_STOP();
					Flag=1;
				}
			}
			else
			{
				over_charge_cnt=0;
				et=adc_I1-adc_I1_Set;
				Et=et;
				if((et>-50)&&(et<50))
				{
					if(k>1) k--;
				}
				else k=10;
				if(PwmFlag==0)
				{
					Duty=duty_real;
					duty=Duty<<8;
				}
				else
				{
					duty -= k*et;
				}
				if(duty<DUTY_MIN<<8) duty=DUTY_MIN<<8;
				else if(duty>DUTY_MAX<<8) duty=DUTY_MAX<<8;
				Duty=duty>>8;
				if(Duty>DUTY_MAX) Duty=DUTY_MAX;
				if(Duty<=DUTY_MIN) 
				{
					Duty=DUTY_MIN;
					PWM_STOP();
				}
				else PWM_START();
			}
		}
	}
	else if(DisChargeMode)
	{
		ONLINE();
		RS_OFF();
//		if(adc_U2_Set<adc_U2_min)
//		{
//			PWM_STOP();
//		}
//		else
		{
			u1=Get_U1(adc_U1,adc_I1);
			if(u1>U1_OF_Set) // 过充保护
			{
				over_charge_cnt++;
				over_discharge_cnt=0;
				if(over_charge_cnt >= 5)// 连续5次过充就关闭
				{
					// 停止充电
					DisChargeMode=0;
					OFFLINE();
					duty=0;
					Duty=0;
					PWM_STOP();
					Flag=1;
				}
			}
			else if(u1<U1_UF_Set) // 过放保护
			{
				over_discharge_cnt++;
				over_charge_cnt=0;
				if(over_discharge_cnt >= 5)// 连续5次过放就关闭
				{
					// 停止放电
					DisChargeMode=0;
					OFFLINE();
					duty=0;
					Duty=0;
					PWM_STOP();
					Flag=2;
				}
			}
			else
			{
				over_charge_cnt=0;
				over_discharge_cnt=0;
				et=adc_U2-adc_U2_Set;
				Et=et;
				if((et>-50)&&(et<50))
				{
					if(k>1) k--;
				}
				else k=10;
				if(PwmFlag==0)
				{
					Duty=DUTY_MAX;
					duty=Duty<<8;
				}
				else
				{
					duty += k*et;
				}
				if(duty<DUTY_MIN<<8) duty=DUTY_MIN<<8;
				else if(duty>DUTY_MAX<<8) duty=DUTY_MAX<<8;
				Duty=duty>>8;
				if(Duty>DUTY_MAX) Duty=DUTY_MAX;
				PWM_START();
			}
		}
	}
	else if(DebugMode)
	{
		OFFLINE();
		over_charge_cnt=0;
		over_discharge_cnt=0;
		RS_ON();
		Duty=DutyRatio*Period/1000;
		PWM_START();
	}
	else 
	{
		OFFLINE();
		over_charge_cnt=0;
		over_discharge_cnt=0;
		duty=0;
		Duty=0;
		PWM_STOP();
	}
	TIM2->CCR1 = Duty;
}*/

void Ctrl(void)
{
	
}

// ????
u16 MidFilter(u16 d1,u16 d2,u16 d3)
{
	u16 result=0;
	if(d1>=d2)
	{
		if(d2>=d3)
		{
			result=d2;
		}
		else 
		{
			if(d1>=d3)
			{
				result=d3;
			}
			else
			{
				result=d1;
			}
		}
	}
	else
	{
		if(d2<=d3)
		{
			result=d2;
		}
		else 
		{
			if(d1>=d3)
			{
				result=d1;
			}
			else
			{
				result=d3;
			}
		}
	}
	return result;
}

s32 adc_ref_mean(s32 di)
{
#define FIFO_SIZE_N	(8)
#define FIFO_SIZE 	(1<<FIFO_SIZE_N)
	static s32 fifo[FIFO_SIZE];
	static u16 p=0;
	static s32 sum=0;
	sum=sum-fifo[p];
	fifo[p]=di;
	sum=sum+di;
	if(p<FIFO_SIZE-1) p++;
	else p=0;
	return (sum>>FIFO_SIZE_N);
}



