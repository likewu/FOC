#ifndef _H_SWITCH
#define _H_SWITCH

#include "define.h"

/*
wx:1076279743
*/
#define SW_SWITCH_NUM 1

#define KEY_EVENT_NULL          0x00
#define KEY_EVENT_3SLONGPRESS   0x01
#define KEY_EVENT_5SLONGPRESS   0x02
#define KEY_EVENT_SHORTRELEASE  0x81
#define KEY_EVENT_3SRELEASE     0x82
#define KEY_EVENT_RELEASEMASK   0x80

#define P_SW_RESET_OFFSET 0

#define P_StartUp_PORT GPIOA
#define P_StartUp_PIN 0

#define SW_OFFSET_START 0
#define SW_START_EVENT() ((g_sw_stRegs.m_uchLevel[SW_OFFSET_START] == TRUE) && (g_sw_stRegs.m_uchTrigger[SW_OFFSET_START] == TRUE))

//------------------------------------------------//

typedef struct {
    U8 m_uchLevel[SW_SWITCH_NUM];
    U8 m_uchTrigger[SW_SWITCH_NUM];
    U8 m_uchCount[SW_SWITCH_NUM];
    U8 m_uchReady;
    U16 m_uiReadyTimer;
    U16 m_uchKeys;
    U8 m_uchEvent[SW_SWITCH_NUM];
} SWStatusType;

typedef struct {
    GPIO_TypeDef *port;
    U8 pin;
    U8 debounce;
    U8 level;
} STRSWConfig;

extern U8 uchStartLevel;
extern SWStatusType g_sw_stRegs;
extern void fnSW_Init(void);
extern void fnSW_RealTime(void);
extern void fnSW_Clear(void);
extern void fnSW_ClearStart(void);
extern U8 fnSW_IsReady(void);
#endif
