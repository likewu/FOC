/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ft1505.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#include "qk_ft1505.h"

void FT1505_Init(void)
{
	u16 i;	
	//配置LCD使用的GPIO
	LCD_InitGPIO();
	//配置FSMC接口
	LCD_FSMCConfig();
	DelayMs(5);
	//初始化液晶
	// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
    LCD_WriteReg(0x0007,0x0000);
    DelayMs(5);
    LCD_WriteReg(0x0012,0x011C);//0x011A   why need to set several times?
    LCD_WriteReg(0x00A4,0x0001);//NVM
    //
    LCD_WriteReg(0x0008,0x000F);
    LCD_WriteReg(0x000A,0x0008);
    LCD_WriteReg(0x000D,0x0008);
   
    //GAMMA CONTROL
    LCD_WriteReg(0x0030,0x0707);
    LCD_WriteReg(0x0031,0x0007); //0x0707
    LCD_WriteReg(0x0032,0x0603); 
    LCD_WriteReg(0x0033,0x0700); 
    LCD_WriteReg(0x0034,0x0202); 
    LCD_WriteReg(0x0035,0x0002); //?0x0606
    LCD_WriteReg(0x0036,0x1F0F);
    LCD_WriteReg(0x0037,0x0707); //0x0f0f  0x0105
    LCD_WriteReg(0x0038,0x0000); 
    LCD_WriteReg(0x0039,0x0000); 
    LCD_WriteReg(0x003A,0x0707); 
    LCD_WriteReg(0x003B,0x0000); //0x0303
    LCD_WriteReg(0x003C,0x0007); //?0x0707
    LCD_WriteReg(0x003D,0x0000); //0x1313//0x1f08
    DelayMs(5);
    LCD_WriteReg(0x0007,0x0001);
    LCD_WriteReg(0x0017,0x0001); //Power supply startup enable
    DelayMs(5);

    //power control//
    LCD_WriteReg(0x0010,0x17A0); 
    LCD_WriteReg(0x0011,0x0217); //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
    LCD_WriteReg(0x0012,0x011E); //0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
    LCD_WriteReg(0x0013,0x0F00); //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
    LCD_WriteReg(0x002A,0x0000);  
    LCD_WriteReg(0x0029,0x000A); //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
    LCD_WriteReg(0x0012,0x013E); // 0x013C  power supply on
    //Coordinates Control//
    LCD_WriteReg(0x0050,0x0000); //0x0e00
    LCD_WriteReg(0x0051,0x00EF); 
    LCD_WriteReg(0x0052,0x0000); 
    LCD_WriteReg(0x0053,0x013F); 
    //Pannel Image Control//
    LCD_WriteReg(0x0060,0x2700); 
    LCD_WriteReg(0x0061,0x0001); 
    LCD_WriteReg(0x006A,0x0000); 
    LCD_WriteReg(0x0080,0x0000); 
    //Partial Image Control//
    LCD_WriteReg(0x0081,0x0000); 
    LCD_WriteReg(0x0082,0x0000); 
    LCD_WriteReg(0x0083,0x0000); 
    LCD_WriteReg(0x0084,0x0000); 
    LCD_WriteReg(0x0085,0x0000); 
    //Panel Interface Control//
    LCD_WriteReg(0x0090,0x0013); //0x0010 frenqucy
    LCD_WriteReg(0x0092,0x0300); 
    LCD_WriteReg(0x0093,0x0005); 
    LCD_WriteReg(0x0095,0x0000); 
    LCD_WriteReg(0x0097,0x0000); 
    LCD_WriteReg(0x0098,0x0000); 

    LCD_WriteReg(0x0001,0x0100); 
    LCD_WriteReg(0x0002,0x0700); 
    LCD_WriteReg(0x0003,0x1030); 
    LCD_WriteReg(0x0004,0x0000); 
    LCD_WriteReg(0x000C,0x0000); 
    LCD_WriteReg(0x000F,0x0000); 
    LCD_WriteReg(0x0020,0x0000); 
    LCD_WriteReg(0x0021,0x0000); 
    LCD_WriteReg(0x0007,0x0021); 
    DelayMs(20);
    LCD_WriteReg(0x0007,0x0061); 
    DelayMs(20);
    LCD_WriteReg(0x0007,0x0173); 
    DelayMs(20);

  	LCD_BackLightInit();
}
void FT1505_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WriteReg(0x0020,Ypos);        //行
	LCD_WriteReg(0x0021,0x13f-Xpos);  //列
}
void FT1505_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend)
{
	LCD_WriteReg(0x0050,Ypos);
	LCD_WriteReg(0x0052, 319-Xend);
	LCD_WriteReg(0x0051, Yend);
	LCD_WriteReg(0x0053, 319-Xpos);
	//设置光标位置
  	FT1505_SetCursor(Xpos, Ypos);
}

void FT1505_SetPoint(u16 x,u16 y,u16 point)
{
	//设置光标位置
	LCD_WriteReg(0x0020,y);        //行
	LCD_WriteReg(0x0021,0x13f-x);  //列
	//开始内存写入
	FT1505_WriteRAM_Prepare(); 
	//写入颜色值
	LCD_WriteRam(point); 
}

u16  FT1505_GetPoint(u16 x,u16 y)
{ 
	return 0;
}

void FT1505_SetScanMode(u8 xyscan)
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