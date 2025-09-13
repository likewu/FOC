#ifndef __ILI8989_H
#define __ILI8989_H

#include "power.h"

#define ILI8989_WriteRAM_Prepare()	WriteCommand(34)
#define ILI8989_ReadRAM_Prepare()	WriteCommand(34)
void ILI8989_Init(void);
void ILI8989_SetCursor(u16 Xpos, u16 Ypos);
void ILI8989_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend);
void ILI8989_SetScanMode(u8 xyscan);

#endif