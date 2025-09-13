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
//#define MENU_TITLE			("�˵�")								// ���Ĳ˵�����
#define MENU_TITLE				("MENU")								// Ӣ�Ĳ˵�����
#define MENU_TITLE_CHAR_WIDTH 	(Font_ASCII_HEIGHT>>1)					// �˵������ַ����
#define MENU_TITLE_COLOR		(RED)									// �˵�������ɫ
#define MENU_TITLE_BKCOLOR		(CYAN)									// ������ɫ
#define MENU_BKCOLOR			(GRAY)									// ������ɫ
#define MENU_CHARCOLOR			(BLACK)									// ������ɫ
#define MENU_SELECOLOR			(BLUE)									// ѡ�е���ɫ
#define MENU_SIZE 				(sizeof(VarMenu)/sizeof(MENU_MEMBER))	// �˵�������Ԫ�ظ��� 
#define MENU_LINE				(12) 	// ����һ�������ʾ MENU_LINE ��
#define MENU_CHARNUM			(30)	// ˮƽ��ʾ���ַ���
#define MENU_BORDER				(4)		// �߿���
#define MENU_WIDTH		(MENU_CHARNUM*Font_ASCII_WIDTH+MENU_BORDER*4)	// �˵�������
#define MENU_HEIGHT		(Font_ASCII_HEIGHT*((MENU_LINE>MENU_SIZE?MENU_SIZE:MENU_LINE)+1))	// �˵�����߶�
#define MENU_X0			((LCD_H_Pixls-MENU_WIDTH)>>1)		// �˵�������ʼx����
#define	MENU_X1			(MENU_X0+MENU_WIDTH-1)				// �˵�������ֹx����
#define MENU_Y0			((LCD_V_Pixls-MENU_HEIGHT)>>1) 		// �˵�������ʼy����
#define MENU_Y1			(MENU_Y0+MENU_HEIGHT-1)				// �˵�������ֹy����

#define MENU_UPDATE_TIME		(200)	// �˵���ʾ�ĸ�����������������������ڣ� 

void InitParameter(void);
void SetParameter();

#endif