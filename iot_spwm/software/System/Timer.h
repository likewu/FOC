#ifndef __TIMER_H
#define __TIMER_H
#include "define.h"

void Timer_Init(void);

void TIM4_Int_Init(u16 arr, u16 psc);

#define TM_TIMER_NUM 10

typedef union
{
	U8 BYTES[TM_TIMER_NUM];
	struct
	{
		
		U8 bTimer2ms ;
		U8 bTimer8ms ;
		U8 bTimer10ms; 
		U8 bTimer16ms ;
		U8 bTimer20ms ;
		U8 bTimer32ms ;
		U8 bTimer64ms ;
		U8 bTimer100ms ;
		U8 bTimer1s ; 
		U8 bTimer1min; 
	}Bits;
}STRTMRegType;

extern STRTMRegType g_tm_stTimerFlag;
extern void fnTM_Init(void);
extern void fnTM_RealTime(void);
extern void fnTM_Clear(void);

#endif
