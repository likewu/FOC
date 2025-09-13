/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_lcd.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#include "qk_lcd.h"

//�������ȫ�ֱ���
u8 		*Font_ASCII_p=0;//Ӣ������ָ��
u8 		*Font_CHINESE_p=0;//��������ָ��
u8		Font_ASCII_WIDTH=8;//Ӣ��������
u8		Font_ASCII_HEIGHT=16;//Ӣ������߶�
u16		nbyte0=1*16,nbyte1=2*16;

#ifdef USE_DISPLAY_BUFFER
u8		DisplayPos=0;
u16 *	DisplayBuffer=0;
#endif

void Search_Font(u8 *font_data,u32 offset,u8 type);

/*****************************************************************

	�������ܣ�	LCD����
	���룺		
				Color:���������ɫ
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_Clear(u16 Color)
{
  	u32 n;
	u16 *p;
#ifdef USE_DISPLAY_BUFFER
	if(DisplayPos)// ��0��ʾ�ڻ��������
	{
		if(DisplayBuffer)
		{
			n=DISPLAY_BUFFER_XSIZE * DISPLAY_BUFFER_YSIZE;
			p=DisplayBuffer;
			while(n--)
			{
				*p++=Color;
			}
		}
		return;
	}
#endif
	//���ù�������
	LCD_SetDisplayWindow(0,0,LCD_H_Pixls-1,LCD_V_Pixls-1);
	//��ʼ�ڴ�д��
	LCD_WriteRAM_Prepare();
	//д��ָ����ɫ
	n=LCD_H_Pixls*LCD_V_Pixls;
	#ifdef RS_GPIO
		LCD_RS_H();
	#endif
	while(n--)
	{
		LCDRAM=(Color); 
	}

}

/*****************************************************************

	�������ܣ�	����������
	���룺		
				x:��ʼx����
				y:��ʼy����
				ex:�յ�x����
				ey:�յ�y����
				color:�����ɫ
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_SetBar(u16 x,u16 y,u16 ex,u16 ey,u16 color)
{
  	u16 m,n;
	if((x>ex)||(y>ey)) return ;
	//���ù�������
	LCD_SetDisplayWindow(x,y,ex,ey);
	//��ʼ�ڴ�д��
	LCD_WriteRAM_Prepare();
	//д��ָ����ɫ
#ifdef RS_GPIO
	LCD_RS_H();
#endif
	for(m=x;m<ex+1;m++)
	{
		for(n=y;n<ey+1;n++)	
		{
			LCDRAM=(color); 
		}
	}
	//�ָ�ԭ��������
	LCD_SetDisplayWindow(0,0,LCD_H_Pixls-1,LCD_V_Pixls-1);
}

/*****************************************************************

	�������ܣ�	��ȡ����������������ֵ
	���룺		
				x:��ʼx����
				y:��ʼy����
				ex:�յ�x����
				ey:�յ�y����
				*point:������ص��ָ��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_GetBar(u16 x,u16 y,u16 ex,u16 ey,u16 *point)
{
	u16 m,n;
	u16 d;
	if((x>ex)||(y>ey)) return ;
	//���ù�������
	LCD_SetDisplayWindow(x,y,ex,ey);
	//��ʼ�ڴ�д��
	LCD_ReadRAM_Prepare();
	//д��ָ����ɫ
	for(m=x;m<ex+1;m++)
	{
		for(n=y;n<ey+1;n++)	
		{
			d=LCDRAM;
			*point++=d; 
		}
	}
	//�ָ�ԭ��������
	LCD_SetDisplayWindow(0,0,LCD_H_Pixls-1,LCD_V_Pixls-1);
}

/*****************************************************************

	�������ܣ�	����
	���룺		
				x:x����
				y:y����
				color:�����ɫ
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
#ifdef USE_DISPLAY_BUFFER
	if(DisplayPos)// ��0��ʾ�ڻ��������
	{
		if(DisplayBuffer &&(x<DISPLAY_BUFFER_XSIZE)&&(y<DISPLAY_BUFFER_YSIZE))
		{
			DisplayBuffer[y*DISPLAY_BUFFER_XSIZE+x]=color;
		}
		return;
	}
#endif
	//���ù��λ��
	LCD_SetCursor(x,y);
	//��ʼ�ڴ�д��
	LCD_WriteRAM_Prepare(); 
	//д����ɫֵ
	LCD_WriteRam(color); 

}

/*****************************************************************

	�������ܣ�	��ȡһ�����ص�
	���룺		
				x:x����
				y:y����
	���أ�		����ֵ
	��������:	2014-4-2
	��ע��		��

******************************************************************/
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
//	//���ù��λ��
//	LCD_SetCursor(x,y);
//	//��ʼ�ڴ�д��
//	LCD_WriteRAM_Prepare();
//	//��ȡ��ɫֵ 
//	return LCDRAM; 
//}

/*****************************************************************

	�������ܣ�	���ñ�������
	���룺		
				Bright:��������(0~100)
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_SetBright(u8 Bright)
{
#ifdef USE_BACKLIGHT
	TIM4->CCR4 = Bright;
#endif	
}

/*****************************************************************

	�������ܣ�	��ʼ��LCD��FSMC��������
	���룺		��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_InitGPIO(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	
	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG , ENABLE);
	
	/* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	 PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
	 function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

#ifdef  MP3_BOARD
	//PC6->RS  PC7->CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#else
	#ifdef FPGA_STM32_BOARD
		//LCD_RS
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	#elif defined STM32_BD
		/* Set PF0(A0 (RS)) as alternate function push pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOF, &GPIO_InitStructure);
	#else
		/* Set PD11(A16 (RS)) as alternate function push pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	#endif
	/* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
#endif

}

/*****************************************************************

	�������ܣ�	����LCD��FSMCģ��
	���룺		��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	
	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 4 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	p.FSMC_AddressSetupTime = 2;//2;
	p.FSMC_AddressHoldTime = 2;//2;
	p.FSMC_DataSetupTime = 3;// 3
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	
	/* Color LCD configuration ------------------------------------
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
	
	/* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}

/*****************************************************************

	�������ܣ�	LCD����Ƴ�ʼ��
	���룺		��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_BackLightInit(void)
{
#ifdef USE_BACKLIGHT
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//�ȹر�TIM4
	TIM_Cmd(TIM4,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//TIM4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//PB
	
	//�Ȱ�IO�������Ϊ0�����㵱PWMɲ��ʱ����������PWM������ű�Ϊ0
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);					   
	
	//����PB9��ΪPWM����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	//  TIM4 CH4������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	//  PB9   PWM���������в����������ʹ�ܣ����ֻ����ʱ������ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	//  ����ռ�ձȵļĴ�������
	TIM_OCInitStructure.TIM_Pulse =0;//�ߵ�ƽ���

	//  PWM�����������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);

	//  TIM4������																			  
	TIM_TimeBaseStructure.TIM_Period=100-1;	//Ƶ�ʵļ��㣺	72M/(Prescaler+1)/(Period+1)	 
	TIM_TimeBaseStructure.TIM_Prescaler=720-1; 	//Ԥ��ƵΪ0��ʵ�ʼ���ʱΪ0+1���������1��
	TIM_TimeBaseStructure.TIM_ClockDivision=0;			//ʱ�ӷ�ƵΪ0������Ϊ��Ƶ���ʱ��Ƶ��
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	//��TIM4
	TIM_Cmd(TIM4,ENABLE);
	LCD_SetBright(0);
#endif
}

/*****************************************************************

	�������ܣ�	Ѱ����������
	���룺		
				font_data:����������ݵ�ָ��
				offset:�����ļ�ƫ�Ƶ�ַ
				type:��������(type=0��ʾӢ�ģ�type=1��ʾ����)
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void Search_Font(u8 *font_data,u32 offset,u8 type)
{
	u8 r;
	u16 i,j;
	u16 sec_off,byte_off,temp;
	u32 sec;
	u8 *p;
	//ʹ��ϵͳ�ڲ�������
	if(type)//��������
	{
	 	if(Font_CHINESE_p)// ����������ֿ�
		{
			p=Font_CHINESE_p+offset*nbyte1;
			for(i=0;i<nbyte1;i++)   
				font_data[i]=p[i];
		}
		else // ���û�������ֿ�����������ֿ�
		{
			p=(u8*)SpecialFont+offset*nbyte1;
			for(i=0;i<nbyte1;i++)   
				font_data[i]=p[i];
		}
	}
	else //Ӣ������
	{
		offset=offset-0x20;// ȥ��0x20��ǰ�Ŀ����ַ�
		p=Font_ASCII_p+offset*nbyte0;
		for(i=0;i<nbyte0;i++)   
			font_data[i]=p[i];
	} 
}

/*****************************************************************

	�������ܣ�	ѡ������
	���룺		
				font_type_p:��������ָ��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

******************************************************************/
void LCD_SelectFont(const FONT_TYPE  *font_type_p)
{
	u8 n;
	Font_ASCII_p=(u8*)font_type_p->FONT_ASCII_p;
	Font_CHINESE_p=(u8*)font_type_p->FONT_CHINESE_p;
	Font_ASCII_WIDTH=(u8)font_type_p->FONT_ASCII_WIDTH;
	Font_ASCII_HEIGHT=(u8)font_type_p->FONT_ASCII_HEIGHT;
	
	n=Font_ASCII_WIDTH>>3;
	if(Font_ASCII_WIDTH&0x07) n++;
	nbyte0=n*Font_ASCII_HEIGHT;
	if(nbyte0>NBYTE0)nbyte0=NBYTE0;
	n=Font_ASCII_WIDTH>>2;
	if(Font_ASCII_WIDTH&0x03) n++;
	nbyte1=n*Font_ASCII_HEIGHT;
	if(nbyte1>NBYTE1)nbyte1=NBYTE1;
}

/*****************************************************************

	�������ܣ�	��ʾһ���ַ�
	���룺		
				x:�ַ�x����
				y:�ַ�y����
				c:ASCII
				charColor:�ַ���ɫ
				bkColor:������ɫ(-1��ʾ͸����ʾ)
	���أ�		
				0xff:��ʾ�ɹ�
				����:��ʾ���ɹ�
	��������:	2014-4-2
	��ע��		��

******************************************************************/
u8 LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u32 bkColor)
{
	u16 xend,yend,xstart,ystart;
	u8 tmp_char;
	u8 *font_p;
	u8 n;
	u8 font24[NBYTE1];
	if(c<0x20) return c;//�����ַ�ֱ�ӷ���
	xstart=x;
	ystart=y;
	xend=xstart+Font_ASCII_WIDTH;
	yend=ystart+Font_ASCII_HEIGHT;
	Search_Font(font24,c,0);//�����ַ�����
	font_p=font24;
	for(;(y)<yend;(y)++)
	{
  		n=0;
		for(x=xstart;(x)<xend;(x)++)
		{
			if(n==0)
			{
				tmp_char=*font_p++;
				n=7;
			}
			else
			{
				n--;
				tmp_char<<=1;
			}
			if (tmp_char&0x80)
	  		{
	    		LCD_DrawPoint((x),(y),charColor); // �ַ���ɫ
	  		}
			else if(bkColor<0x10000)
			{
	    		LCD_DrawPoint((x),(y),bkColor); // ������ɫ
	  		}
		}
	}
	return 0xff;//�ɹ�����
}

/*****************************************************************

	�������ܣ�	��ʾһ������
	���룺		
				x:����x����
				y:����y����
				c0:GBK���һ���ֽ�
				c1:GBK��ڶ����ֽ�
				charColor:�ַ���ɫ
				bkColor:������ɫ(-1��ʾ͸����ʾ)
	���أ�		
				0:��ʾ�ɹ�
				����:��ʾ���ɹ�
	��������:	2014-4-2
	��ע��		��

******************************************************************/
u8 LCD_PutHanzi(u16 x,u16 y,u8 c0,u8 c1,u16 charColor,u32 bkColor)
{
	u8 str[NBYTE1];
	u8 *p;
	u16 offset;
	u8 n,tmp_char;
	u16 xend,yend,xstart,ystart;
	if(Font_CHINESE_p)
	{
		if(c0 < 0xa1 || c1 < 0xa1) return 1;
		offset=(c0-0xA1)*94+(c1-0xA1);
	}
	else
	{
		offset=c0 & 0x7f;// ȥ�����λ
	}		
	xstart=x;
	ystart=y;
	xend=xstart+(Font_ASCII_WIDTH<<1);
	yend=ystart+Font_ASCII_HEIGHT;
	Search_Font(str,offset,1);
	p=str;
	for(;y<yend;y++)
	{
  		n=0;
		for(x=xstart;x<xend;x++)
		{
			if(n==0)
			{
				tmp_char=*p++;
				n=7;
			}
			else
			{
				n--;
				tmp_char<<=1;
			}
			if (tmp_char&0x80)
	  		{
	    		LCD_DrawPoint(x,y,charColor); // �ַ���ɫ
	  		}
			else if(bkColor<0x10000)
			{
	    		LCD_DrawPoint((x),(y),bkColor); // ������ɫ
	  		}
		}
	}
	return 0;
}

/*****************************************************************

	�������ܣ�	��ʾһ���ַ���
	���룺		
				StartX:�ַ��������ı�����ʼx����
				StartY:�ַ��������ı�����ʼy����
				EndX:�ַ��������ı�����ֹx����
				EndY:�ַ��������ı�����ֹy����
				*c:�ַ���ָ��
				linewidth:�о�
				charColor:�ַ���ɫ
				bkColor:������ɫ(-1��ʾ͸����ʾ)
	���أ�		
				0:��ʾ�ɹ�
				����:��ʾ���ɹ�
	��������:	2014-4-2
	��ע��		��

******************************************************************/
u8 LCD_PutStr(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u8 *c,u8 linewidth,u16 charColor,u32 bkColor)
{

	u16 xend,yend,xstart,ystart,stepx1,stepx2,stepy;
	u16 x,y,x1,y1;
	u8 str[NBYTE1];
	u8 tmp_char;
	u8 *p;
	u8 *pbkup;
	u8 High8bit,Low8bit,n;
	u32 offset;

	x=StartX;
	y=StartY;
	stepx1=Font_ASCII_WIDTH;
	stepx2=2*stepx1;
	stepy=Font_ASCII_HEIGHT+linewidth;
	if((c[0]==0)&&((c[0]>=0x80)&&(x>EndX+1-stepx2)||(c[0]<0x80)&&(x>EndX+1-stepx1)))
	{
		return 1;
	}
	x1=x;
	y1=y;
	while(*c)
	{
		if(c[0]>=0x80)//����
		{	
			if(x>EndX+1-stepx2)//������һ��
			{
				y+=stepy;
				if(y>EndY+1-Font_ASCII_HEIGHT)//����
				{
					return 1;
				}
				x=StartX;
			}
			High8bit=c[0];
		  	Low8bit=c[1];
			LCD_PutHanzi(x,y,High8bit,Low8bit,charColor,bkColor);
			if(Font_CHINESE_p) c+=2;// ����������ֿ�������2���ֽ�
			else c+=1;// ������������ֿ⣬ֻ����1���ֽ�
			x=x+stepx2;
		}
		else//��ʾӢ��
		{
			if(x>EndX+1-stepx1)//������һ��
			{
				
				y+=stepy;
				if(y>EndY+1-Font_ASCII_HEIGHT)//����
				{
					return 1;
				}
				x=StartX;
			}
			if(*c==0x0d)//0x0d  0x0a  :����
			{ 
				x=EndX;
				c++;
				if(*c==0x0a)
				{
					c++;
				}
			}
			else if(*c==0x0a)//0x0d  0x0a  :����
			{ 
				x=EndX;
				c++;
			}
			else if(*c==0x09)//'\t'
			{
				x+=stepx1*4;
				c++;
			}
			else
			{
				LCD_PutChar(x,y,*c,charColor,bkColor);
				c++;
				x+=stepx1;
			}
			
		}
	}
	return 0;
}

/****************************************************************************
	
�������ܣ�	��ָ�����껭ֱ��
	���룺		
			x0     A��������
			y0     A��������
			x1     B��������           
			y1     B��������
			color  ����ɫ
	���أ�	��
	��������:	2014-4-2
	��ע��		��

****************************************************************************/
void LCD_DrawLine(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			LCD_DrawPoint(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			LCD_DrawPoint(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			LCD_DrawPoint(x,y,color);
  		}
 	}
 	else
 	{
 		LCD_DrawPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				LCD_DrawPoint(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				LCD_DrawPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				LCD_DrawPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				LCD_DrawPoint(x, y,color);
    			}
   			}
  		}
 	}
}

/****************************************************************************
	
	�������ܣ�	������ת��Ϊ�ַ���
	���룺		char *str �����洢�ַ���������,
				u32 num ��Ҫת��������,
				char dot С������м�λ
	���أ�		��\0����str���ǵ�ƫ����
	��������:	2014-4-2
	��ע��		����ʵ��:
					char DCvoltage1[20];
					u32 temp16 = 3312;
					char temp;
					temp = num2string(DCvoltage1,temp16,1);
					DCvoltage1[temp] = ' ';
					DCvoltage1[temp+1] = 'V';
					DCvoltage1[temp+2] = '\0';

					���   DCvoltage1 = ��331.2 V��

****************************************************************************/
u8 num2string (char *str,s32 num,char dot)
{
	char weishu=0;
	u32 temp=0;
	char i=0,j=0,flag=0;
	if(!num)
	{
		str[0] = '0';
		str[1] = 0;
		return 1;
	}
	else if(num<0)
	{
		str[0]='-';
		str++;
		flag=1;
		num=-num;
	}
	//����num�Ǽ�λ��	
	temp = num;
	while(temp)
	{
		temp /= 10;
		weishu++;
	}
	temp = num;
	if(weishu<=dot)
	{
		str[0] = '0';
		str[1] = '.';
		for(i=0;i<(dot-weishu);i++)
			str[2+i] = '0';
		for(j=weishu;j>0;j--)
		{	
			str[2+i+j-1] = (temp%10) + 48;
			temp /= 10;
		}
		str[dot + 2 + flag] = 0;
		return (dot + 2 + flag);
	}
	else
	{
		if(dot)
		{
			for(i=weishu+1;i>0;i--)
			{
				if((weishu + 1 - i)<dot)
				{
					str[i - 1] = (temp%10) + 48;
					temp /= 10;
				}
				else if((weishu + 1 - i) == dot)
				{
					str[i - 1] = '.';
				}
				else
				{
					str[i - 1] = (temp%10) + 48;
					temp /= 10;
				}
			}
			str[weishu+1 + flag] = 0;
			return (weishu+1 + flag);
		}
		else
		{
			for(i=weishu;i>0;i--)
			{
				str[i-1] = (temp%10) + 48;
				temp /= 10;
			}
			str[weishu + flag] = 0;
			return weishu + flag;	
		}	
	}
}

/**************************************************************************

	????:	??????????
	??:	
				left,top,right,down:????????????
				mode: 
						0:?????
						1:??????
				Bkcolor:?????Bkcolor?????????
						??0?????
				pic_p:????
	??:		
				0:??
				1: ??????
				2: ????,??????
	????:	2013-7-27
	??:		

***************************************************************************/
u8 LCD_DrawPic(u16 left, u16 top, u16 right,u16 down,u8 mode,u32 Bkcolor,uc8 *pic_p)
{
	u8 flag;
	s16 width,height,xw,yw,x1,x2,y1,y2,temp;
	u16 edition;
	u16 *p16;
	u32 n;
	u32  i;
	u32  j;
	u16 point;
	u32 num;
	if(pic_p==0)return 1;//??????
	width=*(u16 *)(pic_p+2);
	height=*(u16 *)(pic_p+4);
	xw=right-left+1;
	yw=down-top+1;
	if((xw<width)||(yw<height)) return 2;//????,??????
	
	if(mode==0) 
	{
		x1=left;
		y1=top;
	}
	else if(mode==1)
	{
		temp=(xw-width)>>1;
		x1=left+temp;//????x??
		temp=(yw-height)>>1;
		y1=top+temp;//????y??
	}
	
	x2=x1+width-1;//????x??
	y2=y1+height-1;//????y??
	LCD_SetDisplayWindow(x1,y1,x2,y2);
	LCD_WriteRAM_Prepare();
	pic_p+=8;//?????
	p16=(u16*)pic_p;
	edition=*p16;//???????,??V3.0,?0x0300
	p16++;
	num=*(u32*)p16;//??????
	j=width*height;
	//????
	if(((edition==0x0501)||(edition==0x0502))&&(num+6<(j<<2)))//???????????????
	{
		p16+=2;
		n=0;
		for (i=0;i<j;i++)
		{
			if(n)
			{
				n--;
				if(flag)//??????
				{
					point=*p16++;
				}
			}
			else 
			{
			   	n=*(u8*)p16;
				if((n&0x7f) == 0x7f)
				{
					if(n&0x80) flag=0;
					else flag=1;
					p16=(u16*)((u8*)p16+1);
					n=*p16+0x7f;
					p16=(u16*)((u8*)p16+2);
				}
				else 
				{
					if(n&0x80) flag=0;
					else flag=1;
					p16=(u16*)((u8*)p16+1);
					n=n&0x7f;
				}
				n--;
				point=*p16++;
			}	
			LCD_WriteRam(point);
		}
		
	}
	else//????????
	{
		p16--; 
		for(i=0;i<j;i++)
		{
			LCD_WriteRam(*p16++);
		}
	}
	//????
	if(Bkcolor)//????
	{
	 	//????
		if(y1>top)
		{
			LCD_SetDisplayWindow(left,top,right,y1-1);
			LCD_WriteRAM_Prepare();
			j=xw*(y1-top);
			while(j--)
			{
				LCD_WriteRam(Bkcolor);
			}
		}
		//????
		if(x1>left)
		{
			LCD_SetDisplayWindow(left,y1,x1-1,y2);
			LCD_WriteRAM_Prepare();
			j=(x1-left)*(y2-y1+1);
			while(j--)
			{
				LCD_WriteRam(Bkcolor);
			}
		}
		//????
		if(x2<right)
		{
			LCD_SetDisplayWindow(x2+1,y1,right,y2);
			LCD_WriteRAM_Prepare();
			j=(right-x2)*(y2-y1+1);
			while(j--)
			{
				LCD_WriteRam(Bkcolor);
			}
		}
		//????
		if(y2<down)
		{
			LCD_SetDisplayWindow(left,y2+1,right,down);
			LCD_WriteRAM_Prepare();
			j=xw*(down-y2);
			while(j--)
			{
				LCD_WriteRam(Bkcolor);
			}
		}
	}
	//???????
	LCD_SetDisplayWindow(0,0,LCD_H_Pixls-1,LCD_V_Pixls-1);
	return 0;
}