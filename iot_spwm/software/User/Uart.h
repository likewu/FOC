/** Copyright (C) ZZY Electrical Equipment Co,ltd. All Rights Reserved. **/
/****************************************************************************
* File:     Uart.h                                                        *
* Desc:     串口通讯相关定义                                                *
*                                                                           *
* Author:   ZZY                                                     *
* Date:     10-06-24                                                        *
* Notes:                                                                    *
*                                                                           *
*****************************************************************************/


#ifndef _UART_H_
#define _UART_H_

/*-------------------------------- Includes ----------------------------------*/
#include  "define.h"

#define UA_RX_RINGBUFF_SIZE 128

#define UA_RX_BUFF_SIZE 128
#define UA_TX_BUFF_SIZE 128
#define UA_BS_CHARTIME 20

#define UA_MSG_OFFSET_MSGID 0
#define UA_MSG_OFFSET_LENGTH 2
#define UA_MSG_OFFSET_DATA 4
typedef struct
{
    U16 m_uchInP;
    U16 m_uchOutP;
    U16 count;
    char m_uchBuff[UA_RX_BUFF_SIZE];
    char m_uchRingBuff[UA_RX_RINGBUFF_SIZE];
    U8 m_uchCharTime;
    U8 m_uchReceived;
    U16 m_uchLen;
    U16 m_uchPointer;
    U8 m_uchRecvTimer;
    U16 m_uiRecMsgID;

} STRUARecType;

typedef struct
{
    U16 m_uchCount;
    U16 m_uchIndex;
    char m_uchBuff[UA_TX_BUFF_SIZE];
    U8 m_uchIsSending;
    U8 m_uchSendTimer;
} STRUASendType;
extern STRUARecType g_ua_stRecRegs;
extern STRUASendType g_ua_stSendRegs;
extern void fnUA_Init(void);
extern void fnUA_RealTime(void);
extern U8 fnUA_SendReq(U16 msgid, U16 len);

extern U8 fnUA_IsSend(void);
extern void fnUA_Clear(void);
extern void fnUA_TimerInt(void);
#endif
