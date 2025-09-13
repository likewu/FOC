/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ft1505.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __QK_FT1505_H
#define	__QK_FT1505_H

#include "power.h"

#define FT1505_WriteRAM_Prepare()	WriteCommand(0x22)
#define FT1505_ReadRAM_Prepare()	WriteCommand(0x22)
void FT1505_Init(void);
void FT1505_SetCursor(u16 Xpos, u16 Ypos);
void FT1505_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend);
void FT1505_SetPoint(u16 x,u16 y,u16 point);
u16  FT1505_GetPoint(u16 x,u16 y);
void FT1505_SetScanMode(u8 xyscan);

#endif