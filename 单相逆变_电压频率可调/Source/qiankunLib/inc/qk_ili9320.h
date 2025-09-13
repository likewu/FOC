/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ili9320.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __QK_ILI9320_H
#define	__QK_ILI9320_H

#include "power.h"

#define ILI9320_WriteRAM_Prepare()	WriteCommand(34)
#define ILI9320_ReadRAM_Prepare()	WriteCommand(34)
void ILI9320_Init(void);
void ILI9320_SetCursor(u16 Xpos, u16 Ypos);
void ILI9320_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend);
void ILI9320_SetPoint(u16 x,u16 y,u16 point);
u16  ILI9320_GetPoint(u16 x,u16 y);
void ILI9320_SetScanMode(u8 xyscan);

#endif