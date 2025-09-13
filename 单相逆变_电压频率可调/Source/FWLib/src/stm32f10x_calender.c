#include "stm32f10x_calender.h"
#include "stm32f10x_lib.h"
uc16 nmax[61]=      //2004��~~~~~2064���16���Ʊ�ʾũ����Ӧ�·ݵ��������
{0x7551,0x56a0,0xabb6,0x25d0,0x92d0,0xcab4,0xa950,0xb8a0,0xbaa3,0xad50,0x55d8,0x4ba0,0xa5b0,0x517d,0x52b0,0xa930,0x7953,0x6aa0,0xad50,0x5b51,0x4b60,0xa6e5,0xa4e0,0xd260,0xea64,0xd530,0x5aa0,0x76a4,0x96d0,0x4bd6,0x4ad0,0xa4d0,0xd0bd,0xd250,0xd520,0xdd44,0xb5a0,0x56d0,0x55b1,0x49b0,0xa576,0xa4b0,0xaa50,0xb25c,0x6d20,0xada0,0x4b6a,0x9370,0x49f7,0x4970,0x64b0,0x68ad,0xea50,0x6b20,0xa6cb,0xaba0,0x92e0,0xd2e2,0xc960,0xd556,0xd4a0};
/*04	  05	  06   07	   08	  09	 10		 11	   12	 13		14.��9	  15	 16		17	18		19	  20	  21	22		 23	   24	  25	 26		27	   28	  29	 30		31		32	  33	 34	   35	   36	  37	38		39	   40	   41	 42		 43		44	  45	 46	   47	   48	  49	 50		 51		52	  53	54		 55	  56	  57	58		 59	   60	  61	 62		63	   64
ע�ͣ�  Nbs[61]��������λ��000��ʾû�����£�001��ʾ2�£�010��ʾ3�£�011��ʾ4�£�100��ʾ5�£�101��ʾ6�£�110��ʾ7�£�111��ʾ8�¡���4λ��0��ʾ����ΪС��29�죬1��ʾ����Ϊ����30�졣����12λ�ֱ��ʾһ����12���µĴ�С�¡�1����£�0��С�¡�------------���˹�����������Ԫ�ؿ��Խ������ũ����������100�꣬200���������ࡣ����������ġ�
*/

u8 DAT(u8 year,u8 mon)//����ĳ�µ�����
{
	if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12) return 31;
	if(mon!=2) return 30;
	if(year%4==0) return 29;
	return 28;
}

u8 sweek(u8 year,u8 month,u8 day)//��ĳ�����ܼ�,0��ʾ����
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
void qnl(u8 year,u8 month,u8 day,u8 *ny,s8 *nm,u8 *nd)//ͨ��������������nm<0ʱ��ʾ����
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

void Setclock(u8 year,u8 month,u8 day,u8 hour,u8 minute,u8 second)//����2010-01-01��00��00��00��ʱ������RTC_count��,����year>=10
{
	u32 count=0;
	u8 i;
	//��������
	for(i=10;i<year;i++)
	count+=(i%4==0?366:365);
	for(i=1;i<month;i++)
	count+=DAT(year,i);
	count+=day-1;

	//��������
	count*=24*3600;
	count+=hour*3600+minute*60+second;
	/* Wait until last write operation on RTC registers is terminated */
	RTC_WaitForLastTask();

	RTC_SetCounter(count);
	/* Wait until last write operation on RTC registers is terminated */
	RTC_WaitForLastTask();
}

void Readclock(u8 *year,u8 *month,u8 *day,u8 *hour,u8 *minute,u8 *second,u8 *ny,s8 *nm,u8 *nd,u8 *week,u8 str[23])
//��counter��ֵ��2010-01-01��00��00��00��Ӳ��������ʱ��,ͬʱ����ʾ���ݷ���str[23]��,
//��:2011 01 27 23 10 00 2010 12 24 4      nm<0ʱ��ʾ����,week=0��ʾ����
//���� �� �� ��	ʱ �� ��ũ������ ��	����
{
	u32 count=0,days=0;
	u8 i;
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	count=RTC_GetCounter();
	days=count/(3600*24);count%=(3600*24);

	*year=10;*month=1;*day=1;//�趨��ֵ
	for(;;(*year)+=1)
	for(*month=1;*month<=12;(*month)+=1)
	{
		if(days<(i=DAT(*year,*month))) goto there;
		days-=i;
	}
	there:
	*day+=days;//�����������������

	*hour=count/3600;*minute=count%3600/60;*second=count%3600%60;//��ʱ����

	qnl(*year,*month,*day,ny,nm,nd);//��ũ��
								 
	*week=sweek(*year,*month,*day);//��ĳ�����ܼ�,0��ʾ����
	i=0;
	//��ʾ����������
	str[i++]=2;str[i++]=0;str[i++]=*year/10%10;str[i++]=*year%10;
	str[i++]=*month/10%10;str[i++]=*month%10;str[i++]=*day/10%10;str[i++]=*day%10;
	//��ʾʱ����
	str[i++]=*hour/10%10;str[i++]=*hour%10;str[i++]=*minute/10%10;
	str[i++]=*minute%10;str[i++]=*second/10%10;str[i++]=*second%10;
	//��ʾũ��������
	str[i++]=2;str[i++]=0;str[i++]=*ny/10%10;str[i++]=*ny%10;
	str[i++]=*nm/10%10;str[i++]=*nm%10;str[i++]=*nd/10%10;str[i++]=*nd%10;
	//��ʾ����
	str[i++]=*week;

}

		
		
