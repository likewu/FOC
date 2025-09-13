#include "stm32f10x_calender.h"
#include "stm32f10x_lib.h"
uc16 nmax[61]=      //2004年~~~~~2064年的16进制表示农历对应月份的最大天数
{0x7551,0x56a0,0xabb6,0x25d0,0x92d0,0xcab4,0xa950,0xb8a0,0xbaa3,0xad50,0x55d8,0x4ba0,0xa5b0,0x517d,0x52b0,0xa930,0x7953,0x6aa0,0xad50,0x5b51,0x4b60,0xa6e5,0xa4e0,0xd260,0xea64,0xd530,0x5aa0,0x76a4,0x96d0,0x4bd6,0x4ad0,0xa4d0,0xd0bd,0xd250,0xd520,0xdd44,0xb5a0,0x56d0,0x55b1,0x49b0,0xa576,0xa4b0,0xaa50,0xb25c,0x6d20,0xada0,0x4b6a,0x9370,0x49f7,0x4970,0x64b0,0x68ad,0xea50,0x6b20,0xa6cb,0xaba0,0x92e0,0xd2e2,0xc960,0xd556,0xd4a0};
/*04	  05	  06   07	   08	  09	 10		 11	   12	 13		14.闰9	  15	 16		17	18		19	  20	  21	22		 23	   24	  25	 26		27	   28	  29	 30		31		32	  33	 34	   35	   36	  37	38		39	   40	   41	 42		 43		44	  45	 46	   47	   48	  49	 50		 51		52	  53	54		 55	  56	  57	58		 59	   60	  61	 62		63	   64
注释：  Nbs[61]：：低三位：000表示没有闰月，001表示2月，010表示3月，011表示4月，100表示5月，101表示6月，110表示7月，111表示8月。第4位，0表示闰月为小月29天，1表示闰月为大月30天。其于12位分别表示一年中12个月的大小月。1表大月，0表小月。------------按此规则增加数组元素可以将程序的农历计算扩大到100年，200年甚至更多。阳历是万年的。
*/

u8 DAT(u8 year,u8 mon)//计算某月的天数
{
	if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12) return 31;
	if(mon!=2) return 30;
	if(year%4==0) return 29;
	return 28;
}

u8 sweek(u8 year,u8 month,u8 day)//求某天是周几,0表示周日
{
	u8 i;
	u16 week=5;
	for(i=1;10+i<=year;i++)
	{
		if((9+i)%4==0)
			week+=2;
		else week++;
	}
	week%=7;
	for(i=1;i<month;i++)
		week+=DAT(year,i)%7;
	week+=day-1;
	week%=7;
	return week;
	
}

u8 nld(u8 ny,s8 nm,u8 nd)
{
	if(nm<0&&nmax[ny-4]%16) {if(nm!=(-1)&&(-nm)==(nmax[ny-4]%8+1)){if(nmax[ny-4]&8) return 30;else return 29;}else if(nm==-9&&(nmax[ny-4]%8)==0) return 29;}
	if(nm<0) return 0;
	if((nmax[ny-4]<<(nm-1))&0x8000)return 30;
	else return 29;
} 
void qnl(u8 year,u8 month,u8 day,u8 *ny,s8 *nm,u8 *nd)//通过阳历求阴历，nm<0时表示闰月
{
	u16 days=0;
	u8 i;
	for(i=10;i<year;i++)
	days+=(i%4==0?366:365);
	for(i=1;i<month;i++)
	days+=DAT(year,i);
	if(days==0){*ny=9;*nm=11;*nd=17;goto there;}
	days+=day-45;
	
	*ny=10;*nm=1;*nd=1;
	for(;;(*ny)+=1)
	for(*nm=1;*nm<=12;(*nm)+=1)
	{
		if(days<(i=nld(*ny,*nm,*nd))) goto there;
		days-=i;*nm=-*nm;
		if(days<(i=nld(*ny,*nm,*nd))){*nm=-*nm; goto there;}
		days-=i;*nm=-*nm;
	}
	there:
	(*nd)+=days;
}

void Setclock(u8 year,u8 month,u8 day,u8 hour,u8 minute,u8 second)//将与2010-01-01，00：00：00的时间差存在RTC_count中,其中year>=10
{
	u32 count=0;
	u8 i;
	//计算天数
	for(i=10;i<year;i++)
	count+=(i%4==0?366:365);
	for(i=1;i<month;i++)
	count+=DAT(year,i);
	count+=day-1;

	//计算秒数
	count*=24*3600;
	count+=hour*3600+minute*60+second;
	/* Wait until last write operation on RTC registers is terminated */
	RTC_WaitForLastTask();

	RTC_SetCounter(count);
	/* Wait until last write operation on RTC registers is terminated */
	RTC_WaitForLastTask();
}

void Readclock(u8 *year,u8 *month,u8 *day,u8 *hour,u8 *minute,u8 *second,u8 *ny,s8 *nm,u8 *nd,u8 *week,u8 str[23])
//将counter的值与2010-01-01，00：00：00相加并求出具体时间,同时将显示数据放在str[23]中,
//如:2011 01 27 23 10 00 2010 12 24 4      nm<0时表示闰月,week=0表示周日
//阳历 年 月 日	时 分 秒农历年月 日	星期
{
	u32 count=0,days=0;
	u8 i;
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	count=RTC_GetCounter();
	days=count/(3600*24);count%=(3600*24);

	*year=10;*month=1;*day=1;//设定初值
	for(;;(*year)+=1)
	for(*month=1;*month<=12;(*month)+=1)
	{
		if(days<(i=DAT(*year,*month))) goto there;
		days-=i;
	}
	there:
	*day+=days;//至此已求出阳历日期

	*hour=count/3600;*minute=count%3600/60;*second=count%3600%60;//求时分秒

	qnl(*year,*month,*day,ny,nm,nd);//求农历
								 
	*week=sweek(*year,*month,*day);//求某天是周几,0表示周日
	i=0;
	//显示阳历年月日
	str[i++]=2;str[i++]=0;str[i++]=*year/10%10;str[i++]=*year%10;
	str[i++]=*month/10%10;str[i++]=*month%10;str[i++]=*day/10%10;str[i++]=*day%10;
	//显示时分秒
	str[i++]=*hour/10%10;str[i++]=*hour%10;str[i++]=*minute/10%10;
	str[i++]=*minute%10;str[i++]=*second/10%10;str[i++]=*second%10;
	//显示农历年月日
	str[i++]=2;str[i++]=0;str[i++]=*ny/10%10;str[i++]=*ny%10;
	str[i++]=*nm/10%10;str[i++]=*nm%10;str[i++]=*nd/10%10;str[i++]=*nd%10;
	//显示星期
	str[i++]=*week;

}

		
		
