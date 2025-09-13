/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		font.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#ifndef __FONT_H
#define __FONT_H

#include "power.h"

// 最大支持96X96的中文和48X96的英文字体
#define NBYTE0	(48/8*96)	// 一个英文字体占用的字节数
#define NBYTE1	(96/8*96)	// 一个中文字体占用的字节数

//字体属性结构体
typedef struct
{
	void   *FONT_ASCII_p;//英文字体指针
	void   *FONT_CHINESE_p;//中文字体指针
	unsigned char   FONT_ASCII_WIDTH;//英文字体宽度
	unsigned char   FONT_ASCII_HEIGHT;//英文字体高度
}FONT_TYPE;

//字体类型声明
extern unsigned char const SpecialFont[];// 注意特殊字库的尺寸必须和所选字库一致
extern FONT_TYPE const FONT_Default;
extern FONT_TYPE const FONT_16X32;

#endif
