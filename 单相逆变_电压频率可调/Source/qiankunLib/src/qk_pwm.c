/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_pwm.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-9
Description		--

**************************************************************************/
#include "qk_pwm.h"
/*****************************************************************

	函数功能：	定时器5中断配置
	输入：		period：中断周期 Prescaler分频
	返回：		无
	更新日期:	2014-4-9
	备注：		无

******************************************************************/
void TIM5_Config(unsigned short int period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//时钟预分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_Period = period;			        //自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //时钟分频1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQChannel;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM5,ENABLE);
}
/*****************************************************************

	函数功能：	TIM4_PWM初始化,在定时器2中断处理函数中做采集和控制
	输入：		无
	返回：		无
	更新日期:	2014-4-9
	备注：		无

******************************************************************/
/*void PwmInit(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//先关闭TIM2,4
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM4,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4,ENABLE);//TIM2 TIM4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//PA PB
	
	//先把IO的输出设为0，方便当PWM刹车时可以立马让PWM输出引脚变为0
	GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
	
	PWM_STOP();					   
	
	//配置PA0作为PWM输出端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置PA1作为SD输出端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置PB6作为输出端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置PA3作为输出端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		 

	//  TIMx CHn的配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	//  PWM输出，如果有波形输出，则使能，如果只做定时器，则不使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	//  控制占空比的寄存器配置
	TIM_OCInitStructure.TIM_Pulse = 0;//高电平宽度

	//  PWM输出极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

	//  控制占空比的寄存器配置
	TIM_OCInitStructure.TIM_Pulse = PERIOD;//高电平宽度

	//  PWM输出极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);

	//TIM2  TIM4的配置																			  
	TIM_TimeBaseStructure.TIM_Period=PERIOD;		 
	TIM_TimeBaseStructure.TIM_Prescaler=0; 				
	TIM_TimeBaseStructure.TIM_ClockDivision=0;	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	//TIM2的中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	Duty=PERIOD/2;
	DutyRatio=500;
	RS_OFF();
}*/

/*****************************************************************

	函数功能：	TIM8_PWM初始化用于输出互补PWM(带死区)
	输入：		无
	返回：		无
	更新日期:	2014-4-9
	备注：		仅初始化了PA6 、PC7作互补PWM输出(若需其他配置引脚即可)

******************************************************************/
void TIM8_Init()
{
GPIO_InitTypeDef         GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;
NVIC_InitTypeDef        NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);  

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_Init(GPIOC,&GPIO_InitStructure);

TIM_DeInit(TIM8);
TIM_TimeBaseInitStructure.TIM_Period = PERIOD;     
TIM_TimeBaseInitStructure.TIM_Prescaler = 0;  
TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;	
//TIM_ClearFlag(TIM8,TIM_FLAG_Update);
//TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);  	
TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure); 


//NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQChannel;	
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级2
//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //响应优先级1
//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
//NVIC_Init(&NVIC_InitStructure);	

TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
TIM_OCInitStructure.TIM_Pulse =0;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     
TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

TIM_OC1Init(TIM8, &TIM_OCInitStructure);
TIM_OC2Init(TIM8, &TIM_OCInitStructure);
TIM_OC3Init(TIM8, &TIM_OCInitStructure);

TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);

TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
TIM_BDTRInitStructure.TIM_DeadTime = 26;//26
TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;               
TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

TIM_Cmd(TIM8, ENABLE);
 TIM_CCPreloadControl(TIM8,ENABLE);
TIM_CtrlPWMOutputs(TIM8, ENABLE);
}



void UpdateDutyA(s32 dutyA)			
{
	s32 d1,d2;
	d1=dutyA-(DeadTime/2);
	d2=dutyA+(DeadTime/2);
	if(d1<=0)
	{	
		d1=0;
		d2=0;
	}
	else if(d2>=Period)
	{
		d2=Period;
		d1=Period;
	}
	TIM2->CCR1 = d1;
	TIM2->CCR2 = d2;
}

void UpdateDutyB(s32 dutyB)			
{
	s32 d1,d2;
	d1=dutyB-(DeadTime/2);
	d2=dutyB+(DeadTime/2);
	if(d1<=0)
	{	
		d1=0;
		d2=0;
	}
	else if(d2>=Period)
	{
		d2=Period;
		d1=Period;
	}
	TIM2->CCR3 = d1;
	TIM2->CCR4 = d2;
}

void UpdateDutyC(s32 dutyC)			
{
	s32 d1,d2;
	d1=dutyC-(DeadTime/2);
	d2=dutyC+(DeadTime/2);
	if(d1<=0)
	{	
		d1=0;
		d2=0;
	}
	else if(d2>=Period)
	{
		d2=Period;
		d1=Period;
	}
	TIM4->CCR1 = d1;
	TIM4->CCR2 = d2;
}