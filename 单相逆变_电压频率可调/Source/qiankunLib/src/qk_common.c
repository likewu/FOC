/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_common.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-11
Description		--

**************************************************************************/

#include "qk_common.h"

//us延时倍乘数	 
static unsigned char  fac_us=0;
//ms延时倍乘数
static unsigned int fac_ms=0;
//系统使用的时钟频率，根据CloclkInit()来确定
u32 System_Clock=8000000;

void DelayInit(void);

/*****************************************************************

	函数功能：	时钟初始化
	输入：		无
	返回：		无
	更新日期:	2014-4-1
	备注：		无

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
	// 系统任务定时器初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SYS_TASK_TIM , ENABLE);
	//  SYS_TASK_TIM的配置																			  
	TIM_TimeBaseStructure.TIM_Period=SYS_TASK_PERIOD_DEFAULT-1;	//频率的计算：	72M/(Prescaler+1)/(Period+1)	 
	TIM_TimeBaseStructure.TIM_Prescaler=System_Clock/100000-1; //预分频为0，实际计算时为0+1（都需加上1）
	TIM_TimeBaseStructure.TIM_ClockDivision=0;			//时钟分频为0，则仍为倍频后的时钟频率
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(SYS_TASK_TIM,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(SYS_TASK_TIM,ENABLE);
	TIM_Cmd(SYS_TASK_TIM,ENABLE);

}

/*****************************************************************

	函数功能：	延时函数初始化
	输入：		无
	返回：		无
	更新日期:	2014-4-1
	备注：		无

******************************************************************/
void DelayInit(void)
{
	//bit2置0，选择SYSTICK的时钟源为HCLK时钟的1/8
	SysTick->CTRL&=0Xfffb;
	
	//us延时倍乘数，即每微秒SysTick递减多少
	fac_us=System_Clock/1000000/8;		    

	//ms延时倍乘数，即每毫秒SysTick递减多少
	fac_ms=(unsigned int)fac_us*1000;
}

/*****************************************************************

	函数功能：	延时函数初始化
	输入：		
				nms:毫秒数
	返回：		无
	更新日期:	2014-4-1
	备注：		无

******************************************************************/
void DelayMs(u32 nms)
{	 		  	  
	u32 temp;	
	
	//等待时间到达
	while(nms--)
	{
		//时间加载(SysTick->LOAD为24bit)
		SysTick->LOAD=(u32)fac_ms;
		
		//清空计数器
		SysTick->VAL =0x00;
		
		//开始倒数            
		SysTick->CTRL=0x01 ;           
		
		//SysTick->VAL倒数到0后，SysTick->CTRL最高位自动置1，读取该位将自动清0 
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));
	}  
	
	//关闭计数器
	SysTick->CTRL=0x00;       
	
	//清空计数器
	SysTick->VAL =0X00;       	  	    
}  
 
/*****************************************************************

	函数功能：	延时函数初始化
	输入：		
				nus:微秒数
	返回：		无
	更新日期:	2014-4-1
	备注：		无

******************************************************************/	    								   
void DelayUs(u32 nus)
{		
	u32 temp;
	
	//时间加载(SysTick->LOAD为24bit)	    	 
	SysTick->LOAD=nus*fac_us; 
	
	//清空计数器	  		 
	SysTick->VAL=0x00;       
	
	//开始倒数
	SysTick->CTRL=0x01 ;       	 
	
	//等待时间到达
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	
	//关闭计数器
	SysTick->CTRL=0x00;       
	
	//清空计数器
	SysTick->VAL =0X00;       	 
}

