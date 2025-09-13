#include "DMA.h"
#define DMA_UART_BUFFSIZE 128
U8 g_dma_uartbuff[DMA_UART_BUFFSIZE];

#define DMA_UART_TX_BUFFSIZE 128
U8 g_dma_uarttxbuff[DMA_UART_TX_BUFFSIZE];

#define UART_DMA  ((USART_TypeDef *) USART2_BASE)

typedef struct {
    U8 m_uchSend;
    U8 m_uchEn;
    U16 m_uchRevPointer;
    U16 m_uchRevCount;
} STRDMAUartRegs;
STRDMAUartRegs g_dma_uartregs;

void fnDMA_UARTTxInit(void)
{
    U32 tmpreg, value;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel7->CCR &= ~DMA_CCR1_EN;
    DMA1_Channel7->CCR = 0;
    DMA1_Channel7->CNDTR = 0;
    DMA1_Channel7->CPAR = 0;
    DMA1_Channel7->CMAR = 0;
    DMA1->IFCR |= DMA_Channel4_IT_Mask;
    tmpreg = DMA1_Channel7->CCR;
    tmpreg &= DMA_CCR_CLEAR_Mask;
    tmpreg |= (DMA_DIR_PeripheralDST | DMA_Mode_Normal
               | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable
               | DMA_PeripheralDataSize_Byte| DMA_MemoryDataSize_Byte
               | DMA_Priority_High | DMA_M2M_Disable);
    DMA1_Channel7->CCR = tmpreg;
    DMA1_Channel7->CNDTR = 0;
    DMA1_Channel7->CPAR = (U32)&UART_DMA->DR;
    DMA1_Channel7->CMAR = (U32)g_dma_uarttxbuff;
    DMA1_Channel7->CCR &= ~(DMA_CCR1_EN);

}

void fnDMA_UARTRxInit(void)
{
    U32 tmpreg, value;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel6->CCR &= ~DMA_CCR1_EN;
    DMA1_Channel6->CCR = 0;
    DMA1_Channel6->CNDTR = 0;
    DMA1_Channel6->CPAR = 0;
    DMA1_Channel6->CMAR = 0;
    DMA1->IFCR |= DMA_Channel6_IT_Mask;
    tmpreg = DMA1_Channel6->CCR;
    tmpreg &= DMA_CCR_CLEAR_Mask;
    tmpreg |= (DMA_DIR_PeripheralSRC | DMA_Mode_Circular
               | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable
               | DMA_PeripheralDataSize_Byte| DMA_MemoryDataSize_Byte
               | DMA_Priority_High | DMA_M2M_Disable);
    DMA1_Channel6->CCR = tmpreg;
    DMA1_Channel6->CNDTR = DMA_UART_BUFFSIZE;
    DMA1_Channel6->CPAR = (U32)&UART_DMA->DR;
    DMA1_Channel6->CMAR = (U32)g_dma_uartbuff;
    DMA1_Channel6->CCR |= DMA_CCR1_EN;

}

U8 fnDMA_UART_IsTXIdle(void) {
    U8 res = FALSE;
    if(g_dma_uartregs.m_uchEn == FALSE) {
        res = TRUE;
    }
    return(res);
}
U8 fnDMA_UARTTxStart(U8 *buff, U16 size)
{
    U16 i;
    U8 nv, data;
    U8 res = FALSE;
    static U16 msgid = 0;
    if(g_dma_uartregs.m_uchEn == FALSE) {
        g_dma_uarttxbuff[0] = 0x55;
        g_dma_uarttxbuff[1] = 0xff;
        g_dma_uarttxbuff[2] = (U8)(msgid >> 8);
        g_dma_uarttxbuff[3] = (U8)(msgid & 0xff);
        msgid++;
        g_dma_uarttxbuff[4] = 0;
        g_dma_uarttxbuff[5] = size;
        memcpy(&g_dma_uarttxbuff[6], buff, size);
        g_dma_uarttxbuff[6 + size] = 0x55;
        fnDMA_UARTTxInit();
        DMA1->IFCR |= DMA_ISR_TCIF7;
        DMA1_Channel7->CNDTR = size  + 7;
        DMA1_Channel7->CCR |= DMA_CCR1_EN;
        g_dma_uartregs.m_uchEn = TRUE;
        g_dma_uartregs.m_uchSend = TRUE;
        res = TRUE;

    } else {

    }
    return(res);


}

void fnDMA_UARTInit(void)
{
    memset(&g_dma_uartregs, 0, sizeof(g_dma_uartregs));
    g_dma_uartregs.m_uchRevCount = DMA_UART_BUFFSIZE;
    g_dma_uartregs.m_uchRevPointer = 0;
    fnDMA_UARTRxInit();
    fnDMA_UARTTxInit();
}

void fnDMA_UartRead(U8 *buf, U16 *pointer, U16 size, U16 *count) {
    while(g_dma_uartregs.m_uchRevCount != DMA1_Channel6->CNDTR) {
        if(DMA1_Channel6->CNDTR != 0) {


            if(g_dma_uartregs.m_uchRevCount == 1) {
                g_dma_uartregs.m_uchRevCount = DMA_UART_BUFFSIZE;
            } else {
                g_dma_uartregs.m_uchRevCount --;
            }
            if(g_dma_uartregs.m_uchRevPointer >= DMA_UART_BUFFSIZE) {
                g_dma_uartregs.m_uchRevPointer = 0;
            }
            if(*pointer >= size) {
                *pointer = 0;
            }
            if(*count < size) {
                buf[*pointer] = g_dma_uartbuff[g_dma_uartregs.m_uchRevPointer];
                //	g_dma_uartbuff[g_dma_uartregs.m_uchRevPointer] = 0;
                *pointer += 1;

                if(*pointer >= size) {
                    *pointer = 0;
                }
                *count += 1;
            }

        }
        g_dma_uartregs.m_uchRevPointer ++;
        if(g_dma_uartregs.m_uchRevPointer >= DMA_UART_BUFFSIZE) {
            g_dma_uartregs.m_uchRevPointer = 0;
        }
    }
}
void fnDMA_RealTime(void) {
    if(g_dma_uartregs.m_uchEn) {
        if(DMA1->ISR & DMA_ISR_TCIF7) {
            DMA1_Channel7->CCR &= ~(DMA_CCR1_EN);
            g_dma_uartregs.m_uchEn = FALSE;
        }
    }
}
