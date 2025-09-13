/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ssd1963.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __QK_SSD1963_H
#define	__QK_SSD1963_H

#include "power.h"
 
#define LC_TYPE 2	//1: 5”¢¥Á∆¡	2:7”¢¥Á∆¡

#define XSIZE	LCD_H_Pixls
#define YSIZE	LCD_V_Pixls

#define HDP		(XSIZE-1)
#define HT		928
#define HPS		46
#define LPS		15
#define HPW		48

#define VDP		(YSIZE-1)
#define VT		525
#define VPS		16
#define FPS		8
#define VPW		16

#define SSD1963_WriteRAM_Prepare()	WriteCommand(0x2c)
#define SSD1963_ReadRAM_Prepare()	WriteCommand(0x3e)
void SSD1963_Init(void);
void SSD1963_SetCursor(u16 Xpos, u16 Ypos);
void SSD1963_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend);
void SSD1963_SetPoint(u16 x,u16 y,u16 point);
u16  SSD1963_GetPoint(u16 x,u16 y);
void SSD1963_SetScanMode(u8 xyscan); 

#endif
