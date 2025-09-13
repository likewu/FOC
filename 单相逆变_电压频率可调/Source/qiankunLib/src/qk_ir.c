/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ir.c
Author			qiankunTeam
Version			V1.0.0
Date			2015-4-5
Description		--

**************************************************************************/

#include "qk_ir.h"

u8 IR_Key=0;// 解码出来的遥控器按键值
static u8 IR_Key_Last=0;

/*****************************************************************

	函数功能：	红外遥控器初始化
	输入：		无
	返回：		无
	更新日期:	2014-4-10
	备注：		无

******************************************************************/
void IrInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

	//PC12作为红外信号输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	//外部中断线的配置
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);

	//EXTI_Line12的中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	//解码红外用到的定时器																		  
	TIM_TimeBaseStructure.TIM_Period=150*10-1;	//红外超时长度定义150ms	 
	TIM_TimeBaseStructure.TIM_Prescaler=System_Clock/10000-1; //预分频为0，实际计算时为0+1（都需加上1）
	TIM_TimeBaseStructure.TIM_ClockDivision=0;			//时钟分频为0，则仍为倍频后的时钟频率
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(IR_TIM,&TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(IR_TIM,ENABLE);
	TIM_Cmd(IR_TIM,ENABLE);

}

/*****************************************************************

	函数功能：	红外遥控器解码
	输入：		无
	返回：		无
	更新日期:	2014-4-10
	备注：		将IrDecode()放在相应外部中断线的中断函数中，方便
				解码。

******************************************************************/
void IrDecode(void)
{
	static u8 data1,data2,data3,data4;
	static u8 IR_start=0,IR_n=0;
	u16 IR_counter=IR_TIM->CNT;

	if(IR_TIM->SR & TIM_FLAG_Update)//如果红外超时
	{
		IR_start=-1;
		IR_Key_Last = 0;
		IR_LED_OFF(); 
	}
	else
	{
		if(IR_DATA_IS_HALF)
		{
			return;
		}
		if(IR_DATA_IS_RE)
		{
			IR_Key=IR_Key_Last;
			IR_n=0;
			IR_start=0;
			IR_LED_OFF();
		}
		else if(IR_DATA_IS_START)//有效数据开始
		{
			IR_n=0;
			IR_start=1;
			IR_LED_ON();
		}
		else if(IR_start!=1)
		{
			//没有找到起始位，所以不进行任何处理
		}
		else 
		{
			if(IR_n<8)//第一个字节
			{
				data1 >>= 1;
				if(IR_DATA_IS_L)//低电平
				{
					IR_n++;
				}
				else if(IR_DATA_IS_H)//高电平
				{
					data1 |= 0x80;
					IR_n++;
				}
				else//出错
				{
					IR_start=-1;
				}
				IR_LED_OFF();
			}
			else if(IR_n<16)//第二个字节
			{
				data2 >>= 1;
				if(IR_DATA_IS_L)//低电平
				{
					IR_n++;
				}
				else if(IR_DATA_IS_H)//高电平
				{
					data2 |= 0x80;
					IR_n++;
				}
				else//出错
				{
					IR_start=-1;
				}
				IR_LED_ON();
			}
			else if(IR_n<24)//第三个字节
			{
				data3 >>= 1;
				if(IR_DATA_IS_L)//低电平
				{
					IR_n++;
				}
				else if(IR_DATA_IS_H)//高电平
				{
					data3 |= 0x80;
					IR_n++;
				}
				else//出错
				{
					IR_start=-1;
				}
				IR_LED_OFF();
			}
			else if(IR_n<32)//第四个字节
			{
				data4 >>= 1;
				if(IR_DATA_IS_L)//低电平
				{
					IR_n++;
				}
				else if(IR_DATA_IS_H)//高电平
				{
					data4 |= 0x80;
					IR_n++;
				}
				else//出错
				{
					IR_start=-1;
				}
				IR_LED_ON();
			}
			
			if(IR_n>=32)//按键编码全部保存，开始处理
			{
				IR_start=0;
				IR_LED_OFF();
				if((data1+data2==0xff)&&(data3+data4==0xff))//校验成功
				{
					//获取按键值
					switch(data3)
					{
					 	case 69:IR_Key=1;break;
						case 70:IR_Key=2;break;
						case 71:IR_Key=3;break;

						case 68:IR_Key=4;break;
						case 64:IR_Key=5;break;
						case 67:IR_Key=6;break;

						case 7:IR_Key=7;break;
						case 21:IR_Key=8;break;
						case 9:IR_Key=9;break;

						case 22:IR_Key=10;break;
						case 25:IR_Key=11;break;
						case 13:IR_Key=12;break;

						case 12:IR_Key=13;break;
						case 24:IR_Key=14;break;
						case 94:IR_Key=15;break;

						case 8:IR_Key=16;break;
						case 28:IR_Key=17;break;
						case 90:IR_Key=18;break;

						case 66:IR_Key=19;break;
						case 82:IR_Key=20;break;
						case 74:IR_Key=21;break;
					}
					IR_Key_Last =IR_Key;
				}
				else
				{

				}
			}
			
		}
	}
	// IR所使用的TIM相关后事处理
	IR_TIM->CNT = 0;
	IR_TIM->EGR = TIM_EventSource_Update;
	IR_TIM->SR = (u16)~TIM_FLAG_Update;
}