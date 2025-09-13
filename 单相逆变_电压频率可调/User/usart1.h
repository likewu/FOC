#ifndef __USART1_H
#define	__USART1_H

//#include "stm32f10x.h"
#include "power.h"
#include <stdio.h>

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, unsigned char *Data,...);

#endif /* __USART1_H */
