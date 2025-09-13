/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ili9320.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#include "qk_ili9320.h"

void ILI9320_Init(void)
{
	u16 i;	
	//配置LCD使用的GPIO
	LCD_InitGPIO();
	//配置FSMC接口
	LCD_FSMCConfig();
	DelayMs(5);
	//初始化液晶
	LCD_WriteReg(0x00,0x0000);
	DelayMs(5);
	LCD_WriteReg(0x01,0x0100);	//Driver Output Control.
	LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Control.
	LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.

	LCD_WriteReg(0x04,0x0000);	//Scalling Control.
	LCD_WriteReg(0x08,0x0202);	//Display Control 2.(0x0207)
	LCD_WriteReg(0x09,0x0000);	//Display Control 3.(0x0000)
	LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contol.(0x0000)
	LCD_WriteReg(0x0c,1);	//Extern Display Interface Control 1.(0x0000)
	LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
	LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Control 2.

	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);
	LCD_WriteReg(0x07,0x0101);	//Display Control.
	for(i=50000;i>0;i--);
	for(i=50000;i>0;i--);

	LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
	LCD_WriteReg(0x11,0x0007);				//Power Control 2.(0x0001)
	LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));		//Power Control 3.(0x0138)
	LCD_WriteReg(0x13,0x0b00);				//Power Control 4.
	LCD_WriteReg(0x29,0x0000);				//Power Control 7.

	LCD_WriteReg(0x2b,(1<<14)|(1<<4));
	
	LCD_WriteReg(0x50,0);		//Set X Start.
	LCD_WriteReg(0x51,239);		//Set X End.
	LCD_WriteReg(0x52,0);		//Set Y Start.
	LCD_WriteReg(0x53,319);		//Set Y End.

	LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
	LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
	LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.

	LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
	LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
	LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
	LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
	LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
	LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.

	LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
	LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
	LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
	LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
	LCD_WriteReg(0x97,(0<<8));	//
	LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.

	LCD_WriteReg(0x07,0x0173);	//(0x0173)

  	LCD_BackLightInit();
}
void ILI9320_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WriteReg(0x0020,Ypos);        //行
	LCD_WriteReg(0x0021,0x13f-Xpos);  //列
}
void ILI9320_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend)
{
	LCD_WriteReg(0x0050,Ypos);
	LCD_WriteReg(0x0052, 319-Xend);
	LCD_WriteReg(0x0051, Yend);
	LCD_WriteReg(0x0053, 319-Xpos);
	//设置光标位置
  	ILI9320_SetCursor(Xpos, Ypos);
}

void ILI9320_SetPoint(u16 x,u16 y,u16 point)
{
	//设置光标位置
	LCD_WriteReg(0x0020,y);        //行
	LCD_WriteReg(0x0021,0x13f-x);  //列
	//开始内存写入
	ILI9320_WriteRAM_Prepare(); 
	//写入颜色值
	LCD_WriteRam(point); 
}

u16  ILI9320_GetPoint(u16 x,u16 y)
{
	//设置光标位置
	LCD_WriteReg(0x0020,y);        //行
	LCD_WriteReg(0x0021,0x13f-x);  //列
	//开始读内存
	ILI9320_ReadRAM_Prepare(); 
	return LCDRAM;
}

void ILI9320_SetScanMode(u8 xyscan)
{
	u16 x;
	x=0x1018;
	if(xyscan&0x10)
	{
		x=0x1038;//水平翻转
	}
	else if(xyscan&0x01)
	{
		x=0x1008;//竖直翻转
	}
	LCD_WriteReg(0x03,x);
}