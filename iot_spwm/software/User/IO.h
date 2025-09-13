/******************************************************************************
* Copyright 2014-2015
* FileName: 	 io.h
* Desc: 对IO动作的处理
*
*
* Author:
* Date:
* Notes:
*
* -----------------------------------------------------------------
* Histroy:
*
******************************************************************************/
#ifndef _H_IO_
#define _H_IO_


/*-------------------------------- Includes ----------------------------------*/
#include "define.h"

/*------------------------------ Global Defines ------------------------------*/

typedef struct {
    GPIO_TypeDef *port;
    U8 pin;
} STRIOConfig;





extern GPIO_TypeDef *g_io_GPIORegsTab[];
extern void SET_IO_OUT_HS(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_SET_BIT(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_RESET_BIT(GPIO_TypeDef *port, U8 pin);

extern void SET_IO_OUT(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_IN_PULLUP(GPIO_TypeDef *port,U8 pin);
extern void SET_IO_IN_WITHOUTPULLUP(GPIO_TypeDef *port,U8 pin);
extern void SET_IO_ANALOG(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_OUT_OPEN_DRAIN(GPIO_TypeDef *port, U8 pin);

extern void SET_IO_AFMODE_PP_HS(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_AFMODE_PP(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_HIGH(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_LOW(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_HIGH_OPEN_DRAIN(GPIO_TypeDef *port, U8 pin);
extern void SET_IO_LOW_OPEN_DRAIN(GPIO_TypeDef *port,U8 pin);
extern void SET_IO_TOGGLE(GPIO_TypeDef *port,U8 pin);
extern U8 GET_IO_STA(GPIO_TypeDef *port,U8 pin);
extern U8 GET_IO_OUTSTA(GPIO_TypeDef *port,U8 pin);
extern void SET_IO_OUT_ISR(GPIO_TypeDef *port,U8 pin);
#endif
