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

// ���֧��96X96�����ĺ�48X96��Ӣ������
#define NBYTE0	(48/8*96)	// һ��Ӣ������ռ�õ��ֽ���
#define NBYTE1	(96/8*96)	// һ����������ռ�õ��ֽ���

//�������Խṹ��
typedef struct
{
	void   *FONT_ASCII_p;//Ӣ������ָ��
	void   *FONT_CHINESE_p;//��������ָ��
	unsigned char   FONT_ASCII_WIDTH;//Ӣ��������
	unsigned char   FONT_ASCII_HEIGHT;//Ӣ������߶�
}FONT_TYPE;

//������������
extern unsigned char const SpecialFont[];// ע�������ֿ�ĳߴ�������ѡ�ֿ�һ��
extern FONT_TYPE const FONT_Default;
extern FONT_TYPE const FONT_16X32;

#endif
