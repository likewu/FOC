/*
 * File: Uart.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "Uart.h"
#include "IO.h"
#include "Timer.h"
#include "DMA.h"
#define P_UA_TX_P GPIOA
#define P_UA_TX_V 2
#define P_UA_RX_P GPIOA
#define P_UA_RX_V 3




#define UART_WIFI  ((USART_TypeDef *) USART2_BASE)
STRUARecType g_ua_stRecRegs;
STRUASendType g_ua_stSendRegs;
#define ENABLE_UART_TX() {\
    UART_WIFI->CR1 |= USART_CR1_TXEIE;\
}
//禁止发送中断，使能接收
#define DISABLE_UART_TX() {\
    UART_WIFI->CR1 &= ~USART_CR1_TXEIE;\
    g_ua_stSendRegs.m_uchSendTimer = 0;\
}

#define ENABLE_UART_INT() {\
	NVIC_EnableIRQ(USART2_IRQn);\
}



void fnUA_Init(void);
void fnUA_RealTime(void);
void fnUA_IOInit(void);
void fnUA_RegInit(void);

void fnUA_IOInit(void)
{
    //使能外设时钟
    U16 bandrate;
    SET_IO_AFMODE_PP(P_UA_TX_P, P_UA_TX_V);
    SET_IO_IN_WITHOUTPULLUP(P_UA_RX_P, P_UA_RX_V);

    //复位UART1外设模块
    RCC->APB1RSTR |=  RCC_APB1RSTR_USART2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;
    //使能外设时钟
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    //使能UART功能
    UART_WIFI->CR1 = 0;  //OVER8=0:16bit sample;M=0:8data bits;PCE=0:parity disable;
    UART_WIFI->CR2 = 0;  //ABREN=0:Auto baud rate detection is disabled;one stop bit;
    UART_WIFI->CR3 = 0;  //OVRDIS=0: Overrun enable;ONEBIT=0: Three sample bit method;EIE=0: Error interrupt enable

    // 24M  FCK 配置波特率为19200U; 24000000/19200=1250(0x04e2)
    bandrate = ((U32)SYSCLK_SYS_FREQ / 2 / (U32)115200);
    UART_WIFI->BRR = bandrate;//);

    UART_WIFI->SR |=  (USART_SR_PE |USART_SR_FE|USART_SR_NE |USART_SR_ORE|USART_SR_TC);

    //接收/接收中断使能
    UART_WIFI->CR1 |= USART_CR1_RE;//USART_CR1_RE | USART_CR1_RXNEIE;
    //发送使能
    UART_WIFI->CR1	|= USART_CR1_TE ;  //TCIE = 0;TXEIE = 0;IDLEIE = 0;

    //使能UART外设模块
    UART_WIFI->CR1 |= USART_CR1_UE;
    UART_WIFI->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;
    // ENABLE_UART_INT();
}
void fnUA_RegInit(void)
{
    memset(&g_ua_stRecRegs, 0, sizeof(g_ua_stRecRegs));
    memset(&g_ua_stSendRegs, 0, sizeof(g_ua_stSendRegs));
    g_ua_stRecRegs.m_uiRecMsgID = 0xffff;
}
void fnUA_Init(void)
{
    fnDMA_UARTInit();
    fnUA_RegInit();
    fnUA_IOInit();


}



void USART1_IRQHandler(void)
{

}


#define UA_RX_CHARTIME  8
void fnUA_RecMsg(void)
{
    U16 outp;
    U8 temp;
    static U8 crc = 0;
    U16 msgid=0;
    if(g_tm_stTimerFlag.Bits.bTimer64ms)
    {
        if(g_ua_stRecRegs.m_uchCharTime)
        {
            g_ua_stRecRegs.m_uchCharTime--;
            if(0 == g_ua_stRecRegs.m_uchCharTime)
            {
                g_ua_stRecRegs.m_uchPointer = 0;
            }
        }
    }
    if(FALSE == g_ua_stSendRegs.m_uchIsSending) {
        if(FALSE == g_ua_stRecRegs.m_uchReceived)
        {
            while(g_ua_stRecRegs.count > 0)
            {
                g_ua_stRecRegs.count --;
                outp = g_ua_stRecRegs.m_uchOutP & (UA_RX_RINGBUFF_SIZE - 1);
                g_ua_stRecRegs.m_uchOutP++;
                temp = g_ua_stRecRegs.m_uchRingBuff[outp];
                if(0 == g_ua_stRecRegs.m_uchPointer)
                {
                    if(0x55 == temp)
                    {
                        crc = 0;
                        g_ua_stRecRegs.m_uchPointer = 1;
                    }
                }
                else if(1 == g_ua_stRecRegs.m_uchPointer)
                {
                    if(0xFF == temp)
                    {
                        g_ua_stRecRegs.m_uchPointer = 2;

                    }
                    else if(0x55 != temp)
                    {
                        g_ua_stRecRegs.m_uchPointer = 0;
                    }
                }
                else if(g_ua_stRecRegs.m_uchPointer <= 3)
                {
                    g_ua_stRecRegs.m_uchBuff[g_ua_stRecRegs.m_uchPointer - 2] = temp;
                    g_ua_stRecRegs.m_uchLen = 0;
                    g_ua_stRecRegs.m_uchPointer ++;
                }
                else if(g_ua_stRecRegs.m_uchPointer <= 5)
                {
                    g_ua_stRecRegs.m_uchLen = g_ua_stRecRegs.m_uchLen << 8;
                    g_ua_stRecRegs.m_uchLen |= (U16)temp;
                    g_ua_stRecRegs.m_uchBuff[g_ua_stRecRegs.m_uchPointer - 2] = temp;
                    g_ua_stRecRegs.m_uchPointer ++;
                    if(g_ua_stRecRegs.m_uchLen >= (UA_RX_BUFF_SIZE - 5)) //msgid, length;
                    {
                        g_ua_stRecRegs.m_uchPointer = 0;
                    }
                }
                else
                {
                    g_ua_stRecRegs.m_uchBuff[g_ua_stRecRegs.m_uchPointer - 2] = temp;
                    g_ua_stRecRegs.m_uchPointer++;
                    if(g_ua_stRecRegs.m_uchPointer >= (g_ua_stRecRegs.m_uchLen + 7))
                    {
                        g_ua_stRecRegs.m_uchPointer = 0;
                        if(1)//(TRUE)//(crc == temp)
                        {
                            g_ua_stRecRegs.m_uchRecvTimer = 6;
                            g_ua_stRecRegs.m_uchLen += 5;

                            g_ua_stRecRegs.m_uchReceived = TRUE;


                        }
                        break;

                    }
                }
                crc += temp;
            }
        }
    }
}
void fnUA_Monitor(void)
{

    if(g_tm_stTimerFlag.Bits.bTimer64ms)
    {
        if(g_ua_stSendRegs.m_uchSendTimer)
        {
            g_ua_stSendRegs.m_uchSendTimer --;
            if(0 == g_ua_stSendRegs.m_uchSendTimer)
            {
                fnUA_Init();
            }
        }
    }
}

void fnUA_RealTime(void)
{
    if(g_ua_stSendRegs.m_uchIsSending) {
        if(fnDMA_UART_IsTXIdle() == TRUE) {
            g_ua_stSendRegs.m_uchIsSending = FALSE;
        }
    }
    fnDMA_RealTime();
    fnDMA_UartRead((U8 *)g_ua_stRecRegs.m_uchRingBuff, &g_ua_stRecRegs.m_uchInP, UA_RX_RINGBUFF_SIZE, &g_ua_stRecRegs.count);

    fnUA_Monitor();
    fnUA_RecMsg();
    if(g_tm_stTimerFlag.Bits.bTimer100ms) {
        if(g_ua_stRecRegs.m_uchRecvTimer)
        {
            g_ua_stRecRegs.m_uchRecvTimer--;
            if(0 == g_ua_stRecRegs.m_uchRecvTimer)
            {
                g_ua_stRecRegs.m_uchReceived = FALSE;
            }
        }
    }

}
U8 fnUA_SendReq(U16 msgid, U16 len)
{

    U8 res = FALSE;
    g_ua_stSendRegs.m_uchBuff[0] = 0x55;
    g_ua_stSendRegs.m_uchBuff[1] = 0xFF;
    g_ua_stSendRegs.m_uchBuff[2] = (U8)(msgid >> 8);
    g_ua_stSendRegs.m_uchBuff[3] = (U8)msgid;

    g_ua_stSendRegs.m_uchBuff[4] = (U8)(len >> 8);
    g_ua_stSendRegs.m_uchBuff[5] = (U8)len;
    g_ua_stSendRegs.m_uchCount = len + 6;
    g_ua_stSendRegs.m_uchSendTimer = (200);
    g_ua_stSendRegs.m_uchIndex = 0;
    g_ua_stSendRegs.m_uchIsSending = TRUE;

    res = fnDMA_UARTTxStart((U8 *)g_ua_stSendRegs.m_uchBuff, g_ua_stSendRegs.m_uchCount);


    return(res);

}

U8 fnUA_IsSend(void)
{
    U8 res = TRUE;
    if(FALSE == g_ua_stSendRegs.m_uchIsSending)
    {
        res = FALSE;
    }

    return res;
}


