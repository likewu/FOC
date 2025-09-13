#ifndef _H_DMA_
#define _H_DMA_
#include "define.h"
/*
wx:1076279743
*/
#define DMA_Channel5_IT_Mask    ((U32)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))

#define DMA_Channel4_IT_Mask    ((U32)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA_Channel6_IT_Mask    ((U32)(DMA_ISR_GIF6 | DMA_ISR_TCIF6 | DMA_ISR_HTIF6 | DMA_ISR_TEIF6))

#define DMA_CCR_CLEAR_Mask           ((U32)0xFFFF800F)

#define DMA_TCIE_ENABLE  ((U32)0x00000002)
#define DMA_TCIE_DISABLE  ((U32)0x00000000)


#define DMA_HFIE_ENABLE  ((U32)0x00000004)
#define DMA_HFIE_DISENABLE  ((U32)0x00000000)





#define CR2_EXTTRIG_SWSTART_Set     ((U32)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset   ((U32)0xFFAFFFFF)
#define DMA1_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA1_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA1_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA1_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA1_Channel6_IT_Mask    ((uint32_t)(DMA_ISR_GIF6 | DMA_ISR_TCIF6 | DMA_ISR_HTIF6 | DMA_ISR_TEIF6))

#define DMA2_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))
#define DMA2_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))


extern void fnDMA_UARTInit(void);

extern void fnDMA_UartRead(U8 *buf, U16 *pointer, U16 size, U16 *count);
extern U8 fnDMA_UART_IsTXIdle(void);
extern U8 fnDMA_UARTTxStart(U8 *buff, U16 size);
extern void fnDMA_RealTime(void);
#endif
