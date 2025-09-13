#ifndef _H_APP_
#define _H_APP_
#include "define.h"

#define P_LED_RED_PORT GPIOB
#define P_LED_RED_PIN 0


typedef struct{
	U8 state;
	U16 timer;
	U16 voltage;
	U16 duty;
	U8 remotecontrol;
	U8 remotestart;
}STRAPType;

extern STRAPType g_ap_stRegs;
extern void fnAP_Init(void);
extern void fnAP_RealTime(void);
#endif

