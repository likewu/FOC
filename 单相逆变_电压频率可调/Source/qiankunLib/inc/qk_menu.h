/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_menu.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-10
Description		--

**************************************************************************/
#ifndef __QK_MENU_H
#define __QK_MENU_H

#include "power.h"

//extern const u8 MENU_TITLE[];
//#define MENU_TITLE			("菜单")								// 中文菜单标题
#define MENU_TITLE				("MENU")								// 英文菜单标题
#define MENU_TITLE_CHAR_WIDTH 	(Font_ASCII_HEIGHT>>1)					// 菜单标题字符宽度
#define MENU_TITLE_COLOR		(RED)									// 菜单标题颜色
#define MENU_TITLE_BKCOLOR		(CYAN)									// 背景颜色
#define MENU_BKCOLOR			(GRAY)									// 背景颜色
#define MENU_CHARCOLOR			(BLACK)									// 字体颜色
#define MENU_SELECOLOR			(BLUE)									// 选中的颜色
#define MENU_SIZE 				(sizeof(VarMenu)/sizeof(MENU_MEMBER))	// 菜单数组中元素个数 
#define MENU_LINE				(12) 	// 界面一次最多显示 MENU_LINE 行
#define MENU_CHARNUM			(30)	// 水平显示的字符数
#define MENU_BORDER				(4)		// 边框宽度
#define MENU_WIDTH		(MENU_CHARNUM*Font_ASCII_WIDTH+MENU_BORDER*4)	// 菜单区域宽度
#define MENU_HEIGHT		(Font_ASCII_HEIGHT*((MENU_LINE>MENU_SIZE?MENU_SIZE:MENU_LINE)+1))	// 菜单区域高度
#define MENU_X0			((LCD_H_Pixls-MENU_WIDTH)>>1)		// 菜单区域起始x坐标
#define	MENU_X1			(MENU_X0+MENU_WIDTH-1)				// 菜单区域终止x坐标
#define MENU_Y0			((LCD_V_Pixls-MENU_HEIGHT)>>1) 		// 菜单区域起始y坐标
#define MENU_Y1			(MENU_Y0+MENU_HEIGHT-1)				// 菜单区域终止y坐标

#define MENU_UPDATE_TIME		(200)	// 菜单显示的更新周期数（相对于任务周期） 

void InitParameter(void);
void SetParameter();

#endif