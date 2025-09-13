/******************** (C) COPYRIGHT 2015 qiankunPower ********************

File Name		qk_menu.c
Author			qiankunTeam
Version			V1.0.0
Date			2015-4-4
Description		--

**************************************************************************/

#include "qk_menu.h"

// 按键值(高8位表示按键功能，低8位表示参数)，用户不能改
#define MENU_KEY_NULL		(0x0000)
#define MENU_KEY_NUM		(0x0100)
#define MENU_KEY_EXIT		(0x0200)
#define MENU_KEY_LEFT		(0x0300)
#define MENU_KEY_RIGHT		(0x0400)
#define MENU_KEY_UP			(0x0500)
#define MENU_KEY_DOWN		(0x0600)
#define MENU_KEY_DEC		(0x0700)
#define MENU_KEY_INC		(0x0800)
#define MENU_KEY_UPDATE		(0x0900)
// 一个变量占用的字节数，用户不能改
#define DATA_NBYTE		 (4)			
// 结构体定义，用户不能改
typedef struct
{
    char name[17];//菜单项目名
    int *p;//菜单中变量的地址
    int dp;//小数点位置
    int addr;//该变量在存储器中的偏移地址，实际地址=偏移地址+绝对地址
    int pro;//属性：可以修改的位数   0：表示只读
    int min;//所能取的最小值
    int max;//所能取的最大值
	void (*changeFun)(void);// 数据改变的响应函数
	
}MENU_MEMBER;
// 存储器被初始化标志，用户不能改
#define FLASH_INIT_FLAG	(0xAA55)	// 占用0号偏移地址

// 本地变量
static s32 AutoUpdate=1;
static s32 KeyValue;

u16 FlashWriteStart(void);
u16 FlashWriteData(u32 addr,u32 data);
u16 FlashWriteEnd(void);

// ----------------------- 以下是用户需要修改的 ---------------------

// 存储在flash中的变量的绝对地址
#define FLASH_START_ADDR (0x08000000 | (510*1024)) 	// 在flash中的地址(508KB处)

s32 tim5_period=0;
// 用户添加变量改变的响应函数
void Set_frequency()
{
	tim5_period=(u32)((float)112000000/(float)(samp_number*set_frequency)-1);
	TIM5->ARR=tim5_period;//改变TIM5的周期
}

void fun2()
{
	ChargeMode=0;
	DebugMode=0;
	if(KeyValue) Flag=0;
	adc_U2_Set = Get_U2_ADC(U2_Set);
}

void fun3()
{
	ChargeMode=0;
	DisChargeMode=0;
	if(KeyValue) Flag=0;
}


// 用户添加菜单里面的变量
MENU_MEMBER VarMenu[]=
{
	{"adc_umax",&adc_umax,0,0,0,0,0},
	{"adc_umin",&adc_umin,0,0,0,0,0},
	{"adc_ch0",&adc_ch0,0,0,0,0,0},		
//	{"adc_ch1",&adc_ch1,0,0,0,0,0},
	{"set_frequency",&set_frequency,0,0,2,30,80,Set_frequency},
	{"set_Amp",&Amp,2,0,2,0,91},
	{"tim5_period",&tim5_period,0,0,0,0,0},
	
	/*
	{"U1_UF_Set",&U1_UF_Set,2,32,4,400,350,Config},//小数点后2位数，，可以选择修改的位数4位，最小4.00最大35.00
	{"U1_OF_Set",&U1_OF_Set,2,31,4,400,350,Config},
	// 充电模式
	{"charge mode",&ChargeMode,0,0,1,0,1,fun1},
	{"I1_Set",&I1_Set,2,30,3,0,200,fun1},
	{"U1",&U1,2,0,0,0,5000},
	{"I1",&I1,3,0,0,0,200},
	{"U2",&U2,2,0,0,0,5000},
	{"et",&Et,0,0,0,0,0},
	{"Flag",&Flag,0,0,0,0,0},
	// 放电模式
	{"discharge mode",&DisChargeMode,0,0,1,0,1,fun2},
	{"U2_Set",&U2_Set,2,33,4,800,4000,fun2},//最大8.00最小40.00
	{"U1",&U1,2,0,0,0,5000},
	{"U2",&U2,2,0,0,0,5000},
	{"et",&Et,0,0,0,0,0},
	{"Flag",&Flag,0,0,0,0,0},
	// 调试模式
	{"debug mode",&DebugMode,0,0,1,0,1,fun3},
	{"U1",&U1,2,0,0,0,5000},
	{"I1",&I1,3,0,0,0,200},
	{"U2",&U2,2,0,0,0,5000},
	{"adc_U1",&adc_U1,0,0,0,0,0},
	{"adc_I1",&adc_I1,0,0,0,0,0},
	{"adc_U2",&adc_U2,0,0,0,0,0},
	{"adc_Ref",&adc_Ref,0,0,0,0,0},
	{"k_U1",&k_U1,3,6,4,0,9999,Config},
	{"U1",&U1,2,0,0,0,0},
	{"k_U2",&k_U2,3,7,4,0,9999,Config},
	{"U2",&U2,2,0,0,0,0},
	{"k_I1",&k_I1,4,8,4,0,9999,Config},
//	{"b_I1",&b_I1,0,9,4,0,9999,Config},
	{"I1",&I1,3,0,0,0,0},
	{"DutyRatio",&DutyRatio,1,0,3,100,995},//调试模式下用遥控器手动给的pwm占空比
	*/
};

// 用户修改菜单里变量写入非易失存储器里开始的准备工作代码
u16 FlashWriteStart(void)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(FLASH_START_ADDR);
	return 0;
}

// 用户修改菜单里变量从非易失存储器某个地址装载的代码
s32 FlashReadData(u32 addr)
{
	return *(s32*)addr;
}

// 用户修改菜单里变量写入非易失存储器某个地址的代码
u16 FlashWriteData(u32 addr,u32 data)
{
	FLASH_ProgramWord(addr, data);
	return 0;
}

// 用户修改菜单里变量写入非易失存储器里结束的善后工作代码
u16 FlashWriteEnd(void)
{
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_Lock();
	return 0;
}

// 菜单里变量从非易失存储器里装载的代码，用户不能改 
u16 LoadData(void)
{
	s32 *pflash;
	u16 i;
	pflash = (s32*)(FLASH_START_ADDR);
	if(*pflash != FLASH_INIT_FLAG)// 还没有初始化，不能装载
	{
		return 1;
	}
	else // 读取数据
	{
		for(i=0;i<MENU_SIZE;i++)
		{
			if(VarMenu[i].addr) *(VarMenu[i].p)=FlashReadData((u32)FLASH_START_ADDR+(u32)VarMenu[i].addr*(u32)DATA_NBYTE);
		}
	}
	return 0;
}

// 菜单里变量保存在非易失存储器的代码，用户不能改
u16 SaveData(void)
{
	u16 i;
	FlashWriteStart();
	// 将所有可存储的变量全部存储起来
	for(i=0;i<MENU_SIZE;i++)
	{
		if(VarMenu[i].addr) FlashWriteData((u32)FLASH_START_ADDR+(u32)VarMenu[i].addr*(u32)DATA_NBYTE,*(VarMenu[i].p));
	}
	FlashWriteData(FLASH_START_ADDR,FLASH_INIT_FLAG);
	FlashWriteEnd();
	// 写入完成后重新装载一下，方便验证数据的正确性
	LoadData();
	return 0;
}

// 用户修改菜单里变量定时更新的代码
void UpdatePeriod(void)
{
	U1=Get_U1(adc_U1,adc_I1);
	U2=Get_U2(adc_U2);
	I1=Get_I1(adc_I1);
}

// 用户修改菜单里变量手动更新一次的代码
void UpdateOneTime(void)
{

}

// 用户修改获取按键值的代码
u16 GetKeyValue(void)
{
	u16 key_value;
	key_value = MENU_KEY_NULL;
	if(IR_Key == 1)
	{
		key_value=MENU_KEY_EXIT;
	}
	else if(IR_Key == 2)
	{
		key_value=MENU_KEY_UPDATE;
	}
	else if(IR_Key == 5)
	{
		key_value=MENU_KEY_LEFT;
	}
	else if(IR_Key == 6)
	{
		key_value=MENU_KEY_RIGHT;
	}
	else if(IR_Key == 11)
	{
		key_value=MENU_KEY_UP;
	}
	else if(IR_Key == 12)
	{
		key_value=MENU_KEY_DOWN;
	}
	else if(IR_Key == 8)
	{
		key_value=MENU_KEY_DEC;
	}
	else if(IR_Key == 9)
	{
		key_value=MENU_KEY_INC;
	}
	else if(IR_Key == 10)
	{
		key_value=MENU_KEY_NUM | 0;
	}
	else if((IR_Key >= 13)&&(IR_Key <= 21))
	{
		key_value=MENU_KEY_NUM | (IR_Key-12);
	}
	IR_Key=0;
	if(key_value) TaskWaitFlag=1;
	else TaskWaitFlag=0;
	return key_value;
}

// ---------------   以下是用户不需要修改的部分  --------------------

static s32 sele,menu_p;
static s32 VarMenuBuffer[MENU_SIZE];// 菜单显示的缓存

//number:待显示的数字（最高10位数）   dp :小数的位数    sele：确定选择的第几个数字
void MenuDisplayNumber(u16 x, u16 y, u16 frontColor, u32 backColor,s32 number,u8 dp,u8 sele )
{
	unsigned char i,j,k;
	u8 num[13];
    num[12]=0;
    if(number<0) 
    {
        num[0]='-';
        number=-number;
    }
    else  num[0]=' ';
	if(dp>10) dp=10;
    else if(dp<0) dp=0;  
    if(sele>11) sele=11;
    else if(sele<0) sele=0;  
    if(!sele) k=12;
	for(i=12,j=1;i>1;)
    {
        i--;
        if(dp==(11-i))num[i]='.';
        else 
        {
            num[i]= number%10+'0';
            number=number/10;
            if(sele==j) k=i;
            j++;
        }
        
    }
    if(sele==11) LCD_PutChar(x,y,num[0],backColor,frontColor);
    else LCD_PutChar(x,y,num[0],frontColor,backColor);
    x += Font_ASCII_WIDTH;
	for(i=1,j=0;i<=11;i++)
	{
        if(i==k)
        {  
            LCD_PutChar(x,y,num[i],backColor,frontColor);
            j++;
        }
        else if((num[i] == '0') && (!j) && (num[i+1]!='.'))
        {
            LCD_PutChar(x,y,' ',frontColor,backColor);
        }
        else
        {
            LCD_PutChar(x,y,num[i],frontColor,backColor);
            j++;
        }
        x += Font_ASCII_WIDTH;   
	}	
}

// 初始化菜单参数，一般情况下用户不需要改
void InitParameter(void)
{
	// 从存储器里装载数据
	LoadData();
}

// 设置参数
void SetParameter(void)
{
   	s32 i,j,k,y,y2;
	u8 num_change;
	s32 temp,temp2,temp3;
	u16 update_cnt=MENU_UPDATE_TIME;
	u16 key_value=MENU_KEY_NULL;
	static s32 start=0xff,menu_p_last=-1,sele_last=-1;
	// 更新显示的类型
	// 0:不更新    b0:只更新数值     b1:只刷新选中和不选中     b2:全部更新
	u8 update_type=0x7;
	u8 keyin=0;
	u8 modified_flag=0;
					  
    // 将变量更新到缓存
	for(i=0;i<MENU_SIZE;i++)
	{
		VarMenuBuffer[i]=*(VarMenu[i].p);
	}
	
	// 上边框
	for(i=-MENU_BORDER;i<0;i++)
	{
		LCD_SetBar(MENU_X0+i,MENU_Y0+i,MENU_X1+i,MENU_Y0+i,WHITE);
	}
	// 左边框
	for(i=-MENU_BORDER;i<0;i++)
	{
		LCD_SetBar(MENU_X0+i,MENU_Y0+i,MENU_X0+i,MENU_Y1+i,WHITE);
	}
	// 下边框
	for(i=-MENU_BORDER;i<0;i++)
	{
		LCD_SetBar(MENU_X0+MENU_BORDER,MENU_Y1-i,MENU_X1-i,MENU_Y1-i,BLACK);
	}
	// 右边框
	for(i=-MENU_BORDER;i<0;i++)
	{
		LCD_SetBar(MENU_X1-i,MENU_Y0+MENU_BORDER,MENU_X1-i,MENU_Y1-i,BLACK);
	}
	LCD_SetBar(MENU_X0,MENU_Y0,MENU_X1,MENU_Y0+Font_ASCII_HEIGHT-1,MENU_TITLE_BKCOLOR);// 填充菜单所在矩形区域
	i=sizeof(MENU_TITLE)*MENU_TITLE_CHAR_WIDTH;
	j=(LCD_H_Pixls-i)>>1;
	LCD_PutStr(j,MENU_Y0,j+i-1,MENU_Y0+Font_ASCII_HEIGHT-1,(u8*)MENU_TITLE,0,MENU_TITLE_COLOR,NO_COLOR);//显示标题栏
	
    // 限幅保护
	if(menu_p>=MENU_SIZE) menu_p=MENU_SIZE-1;
    else if(menu_p<0) menu_p=0;
    if(sele>9) sele=9;
    else if(sele<0) sele=0;
	
	while(1)
	{
		if(update_type)
		{
			// ---- 显示 ----
			// 判断更新类型
			j=MENU_Y0+Font_ASCII_HEIGHT;
			if((menu_p>menu_p_last+1)||(menu_p_last>menu_p+1)) update_type |= (1<<2);
			if(menu_p<start)
			{
				start=menu_p;
				update_type |= (1<<2);
			}
			else if(menu_p>=start+MENU_LINE)
			{
				start=menu_p-MENU_LINE+1;
				update_type |= (1<<2);
			}
			else if(menu_p_last!=menu_p)
			{
				y=j+(menu_p_last-start)*Font_ASCII_HEIGHT;
				y2=j+(menu_p-start)*Font_ASCII_HEIGHT;
				update_type |= (1<<1);
			}
			if(update_type & (1<<2)) 
			{
				LCD_SetBar(MENU_X0,MENU_Y0+Font_ASCII_HEIGHT,MENU_X1,MENU_Y1,MENU_BKCOLOR);// 填充菜单所在矩形区域
			}
			else if(update_type & (1<<1)) 
			{
				LCD_SetBar(MENU_X0,y,MENU_X1,y+Font_ASCII_HEIGHT-1,MENU_BKCOLOR);
			}
			// 开始更新
			k=start;
			for(i=0;i<MENU_LINE;i++,k++,j+=Font_ASCII_HEIGHT)
			{
				if(k>=MENU_SIZE) break;
				if(k==menu_p)
				{
					if(update_type & ((1<<2) | (1<<1))) 
					{
						LCD_SetBar(MENU_X0,j,MENU_X1,j+Font_ASCII_HEIGHT-1,MENU_SELECOLOR);
						LCD_PutStr(MENU_X0+MENU_BORDER,j,MENU_X1,j+Font_ASCII_HEIGHT-1,VarMenu[k].name,0,~MENU_CHARCOLOR,NO_COLOR);//菜单项目名
					}
					if(VarMenuBuffer[k] != *(VarMenu[k].p))// 有数据更新
					{
						num_change=1;// 表示要更新数值
					}
					else num_change=0;// 表示数值没有变化
					if(((update_type & ((1<<2) | (1<<1)))==0)&&(num_change || (sele!=sele_last)))// 单纯数值更新
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,MENU_SELECOLOR,MENU_SELECOLOR,VarMenuBuffer[k],VarMenu[k].dp,0);// 清除原来的数据
					}
					if(num_change) VarMenuBuffer[k] = *(VarMenu[k].p);
					if(VarMenu[k].pro)//读写
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,~MENU_CHARCOLOR,MENU_SELECOLOR,VarMenuBuffer[k],VarMenu[k].dp,sele);//显示参数
					}
					else
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,~RED,MENU_SELECOLOR,VarMenuBuffer[k],VarMenu[k].dp,0);//显示参数
					}
				}
				else if(k==menu_p_last)
				{
					LCD_PutStr(MENU_X0+MENU_BORDER,j,MENU_X1,j+Font_ASCII_HEIGHT-1,VarMenu[k].name,0,MENU_CHARCOLOR,NO_COLOR);//菜单项目名
					if(VarMenuBuffer[k] != *(VarMenu[k].p))// 有数据更新
					{
						num_change=1;// 表示要更新数值
					}
					else num_change=0;// 表示数值没有变化
					if(num_change) VarMenuBuffer[k] = *(VarMenu[k].p);
					if(VarMenu[k].pro)//读写
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,MENU_CHARCOLOR,NO_COLOR,VarMenuBuffer[k],VarMenu[k].dp,0);//显示参数
					}
					else
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,RED,NO_COLOR,VarMenuBuffer[k],VarMenu[k].dp,0);//显示参数
					}
				}
				else 
				{
					if(update_type & (1<<2)) LCD_PutStr(MENU_X0+MENU_BORDER,j,MENU_X1,j+Font_ASCII_HEIGHT-1,VarMenu[k].name,0,MENU_CHARCOLOR,NO_COLOR);//菜单项目名
					if(VarMenuBuffer[k] != *(VarMenu[k].p))// 有数据更新
					{
						num_change=1;// 表示要更新数值
					}
					else num_change=0;// 表示数值没有变化
					if(((update_type & (1<<2))==0)&& num_change)// 单纯数值更新
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,MENU_BKCOLOR,MENU_BKCOLOR,VarMenuBuffer[k],VarMenu[k].dp,0);// 清除原来的数据
					}
					if(num_change) VarMenuBuffer[k] = *(VarMenu[k].p);
					if(VarMenu[k].pro)//读写
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,MENU_CHARCOLOR,NO_COLOR,VarMenuBuffer[k],VarMenu[k].dp,0);//显示参数
					}
					else
					{
						MenuDisplayNumber(MENU_X1-(Font_ASCII_WIDTH*12),j,RED,NO_COLOR,VarMenuBuffer[k],VarMenu[k].dp,0);//显示参数
					}
				}

			}
			sele_last = sele;
			menu_p_last=menu_p;
			update_type=0;
		}
		key_value=GetKeyValue();
		KeyValue = key_value;
		//按键处理
		if(key_value == MENU_KEY_NULL)
		{
			
		}
		else if((key_value & 0xff00) == MENU_KEY_EXIT)// 退出 
		{
		  if(modified_flag)//有数据改动，需要保存
		  {
		      SaveData();
		      modified_flag=0;//清除标志
		  }
		  return;
		}
		else if((key_value & 0xff00) == MENU_KEY_LEFT)//左移
		{   
		  if(sele<VarMenu[menu_p].pro) sele++;
		}
		else if((key_value & 0xff00) == MENU_KEY_RIGHT)//右移
		{
		  if(sele>0) sele--;
		}
		else if((key_value & 0xff00) == MENU_KEY_DEC)//减
		{
		  if(VarMenu[menu_p].pro)
		  {
		      if(sele==9)//选择符号位
		      {
		          if(*VarMenu[menu_p].p>0) 
		          {
		              *VarMenu[menu_p].p=-*VarMenu[menu_p].p;
		              modified_flag++;
		          }
		      }
		      else if(sele==0)//对整个数进行操作 
		      {
		          if(*VarMenu[menu_p].p>VarMenu[menu_p].min) 
		          {
		              (*VarMenu[menu_p].p)--;
		              modified_flag++;
		          }
		
		      }
		      else//只对某位进行操作
		      {   
		          for(temp2=1,i=1;i<sele;i++)
		          {
		              temp2*=10;
		          }           
		          temp3=(*VarMenu[menu_p].p/temp2)%10;
		          temp2=*VarMenu[menu_p].p - temp2;
				  if(temp2>=VarMenu[menu_p].min)
				  {
					 *VarMenu[menu_p].p=temp2;
				  }
				  else
				  {
					 *VarMenu[menu_p].p=VarMenu[menu_p].min;
				  }
				  modified_flag++;
		      }
		  }
		
		}
		else if((key_value & 0xff00) == MENU_KEY_INC)//加
		{
		  if(VarMenu[menu_p].pro)
		  {
		      if(sele==9)//选择符号位
		      {
		          if(*VarMenu[menu_p].p<0) 
		          {
		              *VarMenu[menu_p].p=-*VarMenu[menu_p].p;
		              modified_flag++;
		          }
		      }
		      else if(sele==0)//对整个数进行操作 
		      {
		          if(*VarMenu[menu_p].p<VarMenu[menu_p].max) 
		          {
		              (*VarMenu[menu_p].p)++;
		              modified_flag++;
		          }
		    
		      }
		      else//只对某位进行操作
		      {   
		          for(temp2=1,i=1;i<sele;i++)
		          {
		              temp2*=10;
		          }
		          temp3=(*VarMenu[menu_p].p/temp2)%10;
		          temp2=*VarMenu[menu_p].p + temp2;
				  if(temp2<=VarMenu[menu_p].max)
				  {
					  *VarMenu[menu_p].p =temp2;
				  }
				  else
				  {
					 *VarMenu[menu_p].p =VarMenu[menu_p].max;
				  }
				  modified_flag++;
		      }
		  }
		}
		else if((key_value & 0xff00) == MENU_KEY_UP)//上移
		{
		  if(modified_flag)//有数据改动，需要保存
		  {
		      SaveData();
		      modified_flag=0;//清除标志
		  }
		  
		  if(menu_p>0)menu_p--;
		  else menu_p=MENU_SIZE-1;
		  if(sele>VarMenu[menu_p].pro)sele=VarMenu[menu_p].pro;
		  
		}
		else if((key_value & 0xff00) == MENU_KEY_DOWN)//下移
		{
		  if(modified_flag)//有数据改动，需要保存
		  {
		      SaveData();
		      modified_flag=0;//清除标志
		  }
		  if(menu_p<MENU_SIZE-1)menu_p++;
		  else menu_p=0; 
		  if(sele>VarMenu[menu_p].pro)sele=VarMenu[menu_p].pro;
		}
		else if((key_value & 0xff00) == MENU_KEY_NUM)
		{
		  if(VarMenu[menu_p].pro)
		  {
		      temp3=(key_value & 0x00ff);
		      if(sele==0)//对整个数进行操作 
		      {
		          if((VarMenu[menu_p].min<=temp3)&&(VarMenu[menu_p].max>=temp3))
		          {
		              (*VarMenu[menu_p].p)=temp3;
		              modified_flag++;
		          }
		      }
		      else//只对某位进行操作
		      {   
		          for(temp2=1,i=1;i<sele;i++)
		          {
		              temp2*=10;
		          }
		          temp3-=(*VarMenu[menu_p].p/temp2)%10;
		          temp3=*VarMenu[menu_p].p + temp3*temp2;
		          if((VarMenu[menu_p].min<=temp3)&&(VarMenu[menu_p].max>=temp3))
		          {
		              *VarMenu[menu_p].p=temp3;
		              modified_flag++;
		              if(sele>0) sele--;
		          }
		      }
			}
		}
		else if((key_value & 0xff00) == MENU_KEY_UPDATE)
		{
			UpdateOneTime();
			update_type |= (1<<0);
		}
		else key_value = MENU_KEY_NULL;// 无效按键
		if(key_value) 
		{
			update_type |= (1<<0);
			key_value = MENU_KEY_NULL;
		}
		else update_type = 0;
		if(!update_cnt)
		{
			UpdatePeriod();
			update_cnt = MENU_UPDATE_TIME;
			update_type |= (1<<0);
		}
		else update_cnt--;// 更新显示的计数器自减
		// 变量更新的检测
		if(update_type)
		for(k=0;k<MENU_SIZE;k++)
		{
			if((VarMenuBuffer[k] != *(VarMenu[k].p))&&(VarMenu[k].changeFun))// 有数据更新
			{
				VarMenu[k].changeFun();
			}
		}
		WaitTaskEnd();
	}

}

