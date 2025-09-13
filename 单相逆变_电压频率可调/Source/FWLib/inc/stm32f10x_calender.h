/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CALENDER_H
#define __STM32F10x_CALENDER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

u8 DAT(u8 year,u8 mon);//����ĳ�µ�����
u8 sweek(u8 year,u8 month,u8 day);//��ĳ�����ܼ�,0��ʾ����
u8 nld(u8 ny,s8 nm,u8 nd);
void qnl(u8 year,u8 month,u8 day,u8 *ny,s8 *nm,u8 *nd);//ͨ��������������nm<0ʱ��ʾ����
void Setclock(u8 year,u8 month,u8 day,u8 hour,u8 minute,u8 second);//����2010-01-01��00��00��00��ʱ������RTC_count��,����year>=10
void Readclock(u8 *year,u8 *month,u8 *day,u8 *hour,u8 *minute,u8 *second,u8 *ny,s8 *nm,u8 *nd,u8 *week,u8 str[23]);
//��counter��ֵ��2010-01-01��00��00��00��Ӳ��������ʱ��,ͬʱ����ʾ���ݷ���str[23]��,
//��:2011 01 27 23 10 00 2010 12 24 4      nm<0ʱ��ʾ����,week=0��ʾ����
//���� �� �� ��	ʱ �� ��ũ������ ��	����
#endif /* __STM32F10x_CALENDER_H */
