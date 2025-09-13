/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_gpio.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-1
Description		--

**************************************************************************/
#include "qk_gpio.h"

/*****************************************************************

	�������ܣ�	GPIO��ʼ��
	���룺		��
	���أ�		��
	��������:	2014-4-1
	��ע��		��

******************************************************************/
void GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);
	
	// ����key��Ϊ������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	// key0 -> PC13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// key1 -> PE5,key2 -> PE3,key3 -> PE1,key4 -> PE6
	// key5 -> PE4,key6 -> PE2,key7 -> PE0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
								| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	// ����led����Ĭ�ϵ�ƽ
	LED_ALL_OFF();	

	// ����led��Ϊ�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// led0~led7 -> PF0~PF7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
								| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
}
