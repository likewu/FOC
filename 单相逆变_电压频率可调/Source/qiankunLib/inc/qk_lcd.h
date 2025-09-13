/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_lcd.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __QK_LCD_H
#define __QK_LCD_H

#include "power.h"

//#define USE_DISPLAY_BUFFER
#define USE_BACKLIGHT
//#define MP3_BOARD
//#define STM32_BD
//#define FPGA_STM32_BOARD

#ifdef USE_DISPLAY_BUFFER
	#define DISPLAY_BUFFER_XSIZE OSD_XSIZE
	#define DISPLAY_BUFFER_YSIZE OSD_YSIZE
#endif

//液晶屏类型接口函数
#define FT1505 // 液晶屏类型
#if defined ILI9320
	#include "qk_ili9320.h"
	#define LCD_H_Pixls				(320)
	#define LCD_V_Pixls				(240)
	#define LCD_Init				ILI9320_Init
	#define LCD_SetCursor			ILI9320_SetCursor
	#define LCD_WriteRAM_Prepare()	ILI9320_WriteRAM_Prepare()
	#define LCD_ReadRAM_Prepare()	ILI9320_ReadRAM_Prepare()
	#define LCD_SetDisplayWindow	ILI9320_SetDisplayWindow
	#define LCD_SetScanMode(xyscan)	ILI9320_SetScanMode(xyscan)
	#define LCD_ReadPoint			ILI9320_GetPoint
#elif defined ILI8989
	#include "qk_ili8989.h"
	#define LCD_H_Pixls				(320)
	#define LCD_V_Pixls				(240)
	#define LCD_Init				ILI8989_Init
	#define LCD_SetCursor			ILI8989_SetCursor
	#define LCD_WriteRAM_Prepare()	ILI8989_WriteRAM_Prepare()
	#define LCD_ReadRAM_Prepare()	ILI8989_ReadRAM_Prepare()
	#define LCD_SetDisplayWindow	ILI8989_SetDisplayWindow
	#define LCD_SetScanMode(xyscan)	ILI8989_SetScanMode(xyscan)
	#define LCD_ReadPoint			ILI8989_GetPoint
#elif defined SSD1963
	#include "qk_ssd1963.h"
	#define LCD_H_Pixls				(800)
	#define LCD_V_Pixls				(480)
	#define LCD_Init				SSD1963_Init
	#define LCD_SetCursor			SSD1963_SetCursor
	#define LCD_WriteRAM_Prepare()	SSD1963_WriteRAM_Prepare()
	#define LCD_ReadRAM_Prepare()	SSD1963_ReadRAM_Prepare()
	#define LCD_SetDisplayWindow	SSD1963_SetDisplayWindow
	#define LCD_SetScanMode(xyscan)	SSD1963_SetScanMode(xyscan)
	#define LCD_ReadPoint			SSD1963_GetPoint
#elif defined FT1505
	#include "qk_ft1505.h"
	#define LCD_H_Pixls				(320)
	#define LCD_V_Pixls				(240)
	#define LCD_Init				FT1505_Init
	#define LCD_SetCursor			FT1505_SetCursor
	#define LCD_WriteRAM_Prepare()	FT1505_WriteRAM_Prepare()
	#define LCD_ReadRAM_Prepare()	FT1505_ReadRAM_Prepare()
	#define LCD_SetDisplayWindow	FT1505_SetDisplayWindow
	#define LCD_SetScanMode(xyscan)	FT1505_SetScanMode(xyscan)
	#define LCD_ReadPoint			FT1505_GetPoint
#endif
//颜色定义
#define NO_COLOR		0xFFFFFFFF
#define WHITE			0xFFFF
#define BLACK 			0x0000
#define GRAY           	0xF7DE
#define BLUE           	0x001F
#define RED            	0xF800
#define MAGENTA        	0xF81F
#define GREEN          	0x07E0
#define CYAN           	0x7FFF
#define YELLOW         	0xFFE0


#define LCDREG	(*(vu16 *)(0x60000000 | 0x0C000000))
#ifdef STM32_BD
	#define LCDRAM	(*(vu16 *)(0x60000000 | 0x0C000002))
#else
	#define LCDRAM	(*(vu16 *)(0x60000000 | 0x0C020000))
#endif

#if defined MP3_BOARD
	// LCD在FSMC中的定义
	#define RS_GPIO
	#define LCD_RS_H()	(GPIOC->BSRR = GPIO_Pin_6)
	#define LCD_RS_L()	(GPIOC->BRR = GPIO_Pin_6)
	#define LCD_CS_H()	(GPIOC->BSRR = GPIO_Pin_7)
	#define LCD_CS_L()	(GPIOC->BRR = GPIO_Pin_7)
	// 类函数的相关宏
	#define LCD_WriteRam(Value)			{LCD_RS_H();LCDRAM = Value;}
	#define LCD_WriteReg(Reg,Value)		{LCD_RS_L();LCDREG = Reg;LCD_RS_H();LCDRAM = Value;}
	#define LCD_ReadReg(Reg)			(LCD_RS_L(),LCDREG = Reg,LCD_RS_H(),LCDRAM)
	#define WriteCommand(cmd)			{LCD_RS_L();LCDREG = cmd;}
#elif defined FPGA_STM32_BOARD
	#define RS_GPIO
	#define LCD_RS_H()	(GPIOD->BSRR = GPIO_Pin_3)
	#define LCD_RS_L()	(GPIOD->BRR = GPIO_Pin_3)
	#define LCD_WR_DATA(data)	{LCD_RS_H();LCDRAM = data;}
	#define LCD_RD_DATA()		(LCD_RS_H(),LCDRAM)
	#define LCD_WR_REG(data)	{LCD_RS_L();LCDREG = data;}
	#define LCD_WriteRam(Value)			LCD_WR_DATA(Value)
	#define LCD_WriteReg(Reg,Value)		{LCD_WR_REG(Reg);while(!FPGA_IS_READY);LCD_WR_DATA(Value);while(!FPGA_IS_READY);}
	#define LCD_ReadReg(Reg)			(LCD_WR_REG(Reg),LCDRAM)
	#define WriteCommand(cmd)			{LCD_WR_REG(cmd);}
#else
	//LCD在FSMC中的地址定义
	#define LCD_WriteRam(Value)			(LCDRAM = Value)
	#define LCD_WriteReg(Reg,Value)		{LCDREG = Reg;LCDRAM = Value;}
	#define LCD_ReadReg(Reg)			(LCDREG = Reg,LCDRAM)
	#define WriteCommand(cmd)			(LCDREG = cmd)
#endif

	
//字体相关变量
extern u8 		*Font_ASCII_p;//英文字体指针
extern u8 		*Font_CHINESE_p;//中文字体指针
extern u8		Font_ASCII_WIDTH;//英文字体宽度
extern u8		Font_ASCII_HEIGHT;//英文字体高度
#ifdef USE_DISPLAY_BUFFER
	extern u8		DisplayPos;
	extern u16	*	DisplayBuffer;
#endif

//函数声明
void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
void LCD_SetBar(u16 x,u16 y,u16 ex,u16 ey,u16 color);
void LCD_GetBar(u16 x,u16 y,u16 ex,u16 ey,u16 *point);
void LCD_InitGPIO(void);
void LCD_SetBright(u8 Bright);
void LCD_FSMCConfig(void);
void LCD_BackLightInit(void);
//void LCD_SelectFont(const FONT_TYPE  *font_type_p);
u8 LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u32 bkColor);
u8 LCD_PutHanzi(u16 x,u16 y,u8 c0,u8 c1,u16 charColor,u32 bkColor);
u8 LCD_PutStr(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u8 *c,u8 linewidth,u16 charColor,u32 bkColor);
void LCD_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
u8 num2string (char *str,s32 num,char dot);
u8 LCD_DrawPic(u16 left, u16 top, u16 right,u16 down,u8 mode,u32 Bkcolor,uc8 *pic_p);

#endif 

