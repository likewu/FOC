/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_pwm.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-9
Description		--

**************************************************************************/
#include "qk_pwm.h"
/*****************************************************************

	�������ܣ�	��ʱ��5�ж�����
	���룺		period���ж����� Prescaler��Ƶ
	���أ�		��
	��������:	2014-4-9
	��ע��		��

******************************************************************/
void TIM5_Config(unsigned short int period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_Period = period;			        //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM5_IRQChannel;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM5,ENABLE);
}
/*****************************************************************

	�������ܣ�	TIM4_PWM��ʼ��,�ڶ�ʱ��2�жϴ����������ɼ��Ϳ���
	���룺		��
	���أ�		��
	��������:	2014-4-9
	��ע��		��

******************************************************************/
/*void PwmInit(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//�ȹر�TIM2,4
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM4,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4,ENABLE);//TIM2 TIM4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//PA PB
	
	//�Ȱ�IO�������Ϊ0�����㵱PWMɲ��ʱ����������PWM������ű�Ϊ0
	GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
	
	PWM_STOP();					   
	
	//����PA0��ΪPWM����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����PA1��ΪSD����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����PB6��Ϊ����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����PA3��Ϊ����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		 

	//  TIMx CHn������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	//  PWM���������в����������ʹ�ܣ����ֻ����ʱ������ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	//  ����ռ�ձȵļĴ�������
	TIM_OCInitStructure.TIM_Pulse = 0;//�ߵ�ƽ���

	//  PWM�����������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

	//  ����ռ�ձȵļĴ�������
	TIM_OCInitStructure.TIM_Pulse = PERIOD;//�ߵ�ƽ���

	//  PWM�����������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);

	//TIM2  TIM4������																			  
	TIM_TimeBaseStructure.TIM_Period=PERIOD;		 
	TIM_TimeBaseStructure.TIM_Prescaler=0; 				
	TIM_TimeBaseStructure.TIM_ClockDivision=0;	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	//TIM2���ж�����
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

	�������ܣ�	TIM8_PWM��ʼ�������������PWM(������)
	���룺		��
	���أ�		��
	��������:	2014-4-9
	��ע��		����ʼ����PA6 ��PC7������PWM���(���������������ż���)

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
//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�2
//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //��Ӧ���ȼ�1
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