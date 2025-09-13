#include "timer.h"                  // Device header
#include "gizwits_product.h"

STRTMRegType g_tm_stTimerFlag;
STRTMRegType tm_sTimerFlag;
#define TM_INT_LOAD (SYSCLK_SYS_FREQ / 2000)//2000-500us, 4000-250us
void fnTM_Init(void);
void fnTM_RealTime(void);
void fnTM_Clear(void);

U8 tm_uchTimer2msCount;
U8 tm_uchTimer16msCount;

#define TM_INTERRUPT_TIME_US (U32)500
void Timer_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//开启TIM2的时钟
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM1);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 500 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	
	
	/*中断输出配置*/
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);					//开启TIM2的更新中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*TIM使能*/
	TIM_Cmd(TIM1, ENABLE);			//使能TIM2，定时器开始运行
}


void TIM4_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Ê±ÖÓÊ¹ÄÜ


    //¶¨Ê±Æ÷TIM4³õÊ¼»¯
    TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE ); //Ê¹ÄÜÖ¸¶¨µÄTIM4ÖÐ¶Ï,ÔÊÐí¸üÐÂÖÐ¶Ï

    //ÖÐ¶ÏÓÅÏÈ¼¶NVICÉèÖÃ
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4ÖÐ¶Ï
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //´ÓÓÅÏÈ¼¶3¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);  //³õÊ¼»¯NVIC¼Ä´æÆ÷

    TIM_Cmd(TIM4, ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷4
}
extern u8 wifi_sta;
extern u8 wifi_flag;


u32 wifi_ms;
void TIM4_IRQHandler(void)   //TIM4ÖÐ¶Ï
{
	
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //¼ì²éTIM4¸üÐÂÖÐ¶Ï·¢ÉúÓë·ñ
    {

        gizTimerMs();

    }
	
	
	
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //Çå³ýTIM4¸üÐÂÖÐ¶Ï±êÖ¾
}
void TIM1_UP_IRQHandler(void)       // 每1毫秒进一次中断
{
	static U8 uchTimer500us = 0;
	static U8 uchTimer10mscount = 0;
	static U8 uchTimer100mscounter = 0;	
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        
           
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
	uchTimer500us ++;
	if(0 == (uchTimer500us & 0x03))
	{
			uchTimer10mscount ++;
			uchTimer100mscounter ++;
	    tm_sTimerFlag.Bits.bTimer2ms = TRUE;
	    tm_uchTimer2msCount ++;
	    if(!(tm_uchTimer2msCount & 0x03))
	    {
	    	tm_sTimerFlag.Bits.bTimer8ms = TRUE;
	    }
	    if(!(tm_uchTimer2msCount & 0x07))
	    {
	    	tm_sTimerFlag.Bits.bTimer16ms = TRUE;

	    }
		if(uchTimer10mscount >= 5)
		{
			tm_sTimerFlag.Bits.bTimer10ms = TRUE;
			uchTimer10mscount = 0;
		}
		if(uchTimer100mscounter >= 50){
			tm_sTimerFlag.Bits.bTimer100ms = TRUE;
			uchTimer100mscounter = 0;
		}
	}
}

void fnTM_Init(void)
{
		
		memset(&g_tm_stTimerFlag, 0, sizeof(g_tm_stTimerFlag));
	
}
void fnTM_RealTime(void)
{
	static U16 tm_uchTimer16msCount = 0;
	
	memcpy(&g_tm_stTimerFlag, &tm_sTimerFlag, sizeof(tm_sTimerFlag));
	memset(&tm_sTimerFlag, 0, sizeof(tm_sTimerFlag));
	
	if(g_tm_stTimerFlag.Bits.bTimer16ms)
	{
		tm_uchTimer16msCount ++;		
		if(0 == (tm_uchTimer16msCount  & 0x0001))
		{
			g_tm_stTimerFlag.Bits.bTimer32ms = TRUE;
		}
		if(0 == (tm_uchTimer16msCount  & 0x0003)) // * 4
		{
			g_tm_stTimerFlag.Bits.bTimer64ms = TRUE;
		}
		if(0 == (tm_uchTimer16msCount  & 0x003F))//5
		{
			g_tm_stTimerFlag.Bits.bTimer1s = TRUE;
		}
		if(0 == (tm_uchTimer16msCount  & 0x0FFF))//5
		{
			
			g_tm_stTimerFlag.Bits.bTimer1min = TRUE;
		}
			
	}
}
void fnTM_Clear(void)
{
	memset(g_tm_stTimerFlag.BYTES, 0, TM_TIMER_NUM);
}

void SysTick_Handler(void)
{
	
}


