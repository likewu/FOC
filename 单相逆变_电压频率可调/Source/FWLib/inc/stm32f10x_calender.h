/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CALENDER_H
#define __STM32F10x_CALENDER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

u8 DAT(u8 year,u8 mon);//计算某月的天数
u8 sweek(u8 year,u8 month,u8 day);//求某天是周几,0表示周日
u8 nld(u8 ny,s8 nm,u8 nd);
void qnl(u8 year,u8 month,u8 day,u8 *ny,s8 *nm,u8 *nd);//通过阳历求阴历，nm<0时表示闰月
void Setclock(u8 year,u8 month,u8 day,u8 hour,u8 minute,u8 second);//将与2010-01-01，00：00：00的时间差存在RTC_count中,其中year>=10
void Readclock(u8 *year,u8 *month,u8 *day,u8 *hour,u8 *minute,u8 *second,u8 *ny,s8 *nm,u8 *nd,u8 *week,u8 str[23]);
//将counter的值与2010-01-01，00：00：00相加并求出具体时间,同时将显示数据放在str[23]中,
//如:2011 01 27 23 10 00 2010 12 24 4      nm<0时表示闰月,week=0表示周日
//阳历 年 月 日	时 分 秒农历年月 日	星期
#endif /* __STM32F10x_CALENDER_H */
