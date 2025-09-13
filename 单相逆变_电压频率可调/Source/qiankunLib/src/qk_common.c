/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_common.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-11
Description		--

**************************************************************************/

#include "qk_common.h"

//us��ʱ������	 
static unsigned char  fac_us=0;
//ms��ʱ������
static unsigned int fac_ms=0;
//ϵͳʹ�õ�ʱ��Ƶ�ʣ�����CloclkInit()��ȷ��
u32 System_Clock=8000000;

void DelayInit(void);

/*****************************************************************

	�������ܣ�	ʱ�ӳ�ʼ��
	���룺		��
	���أ�		��
	��������:	2014-4-1
	��ע��		��

******************************************************************/
void CloclkInit(void)
{
	ErrorStatus HSEStartUpStatus;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		
		/* HCLK = System_Clock */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		/* PLLCLK = 8MHz * 14 = 112 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_14);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
		System_Clock = 112000000;
	}
	else System_Clock = 8000000;
	DelayInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	// ϵͳ����ʱ����ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SYS_TASK_TIM , ENABLE);
	//  SYS_TASK_TIM������																			  
	TIM_TimeBaseStructure.TIM_Period=SYS_TASK_PERIOD_DEFAULT-1;	//Ƶ�ʵļ��㣺	72M/(Prescaler+1)/(Period+1)	 
	TIM_TimeBaseStructure.TIM_Prescaler=System_Clock/100000-1; //Ԥ��ƵΪ0��ʵ�ʼ���ʱΪ0+1���������1��
	TIM_TimeBaseStructure.TIM_ClockDivision=0;			//ʱ�ӷ�ƵΪ0������Ϊ��Ƶ���ʱ��Ƶ��
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(SYS_TASK_TIM,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(SYS_TASK_TIM,ENABLE);
	TIM_Cmd(SYS_TASK_TIM,ENABLE);

}

/*****************************************************************

	�������ܣ�	��ʱ������ʼ��
	���룺		��
	���أ�		��
	��������:	2014-4-1
	��ע��		��

******************************************************************/
void DelayInit(void)
{
	//bit2��0��ѡ��SYSTICK��ʱ��ԴΪHCLKʱ�ӵ�1/8
	SysTick->CTRL&=0Xfffb;
	
	//us��ʱ����������ÿ΢��SysTick�ݼ�����
	fac_us=System_Clock/1000000/8;		    

	//ms��ʱ����������ÿ����SysTick�ݼ�����
	fac_ms=(unsigned int)fac_us*1000;
}

/*****************************************************************

	�������ܣ�	��ʱ������ʼ��
	���룺		
				nms:������
	���أ�		��
	��������:	2014-4-1
	��ע��		��

******************************************************************/
void DelayMs(u32 nms)
{	 		  	  
	u32 temp;	
	
	//�ȴ�ʱ�䵽��
	while(nms--)
	{
		//ʱ�����(SysTick->LOADΪ24bit)
		SysTick->LOAD=(u32)fac_ms;
		
		//��ռ�����
		SysTick->VAL =0x00;
		
		//��ʼ����            
		SysTick->CTRL=0x01 ;           
		
		//SysTick->VAL������0��SysTick->CTRL���λ�Զ���1����ȡ��λ���Զ���0 
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));
	}  
	
	//�رռ�����
	SysTick->CTRL=0x00;       
	
	//��ռ�����
	SysTick->VAL =0X00;       	  	    
}  
 
/*****************************************************************

	�������ܣ�	��ʱ������ʼ��
	���룺		
				nus:΢����
	���أ�		��
	��������:	2014-4-1
	��ע��		��

******************************************************************/	    								   
void DelayUs(u32 nus)
{		
	u32 temp;
	
	//ʱ�����(SysTick->LOADΪ24bit)	    	 
	SysTick->LOAD=nus*fac_us; 
	
	//��ռ�����	  		 
	SysTick->VAL=0x00;       
	
	//��ʼ����
	SysTick->CTRL=0x01 ;       	 
	
	//�ȴ�ʱ�䵽��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	
	//�رռ�����
	SysTick->CTRL=0x00;       
	
	//��ռ�����
	SysTick->VAL =0X00;       	 
}

