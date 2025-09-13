/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_ssd1963.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-2
Description		--

**************************************************************************/
#include "qk_ssd1963.h"

void SSD1963_Init(void)
{
	u16 i;
	//配置LCD使用的GPIO
	LCD_InitGPIO();
	//配置FSMC接口
	LCD_FSMCConfig();
	DelayMs(50);
	//初始化液晶
#if(LCD_TYPE == 1)// 5英寸屏
	LCDREG = 0x0001;// software reset 
	DelayMs(200);
	LCDREG = (0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCDRAM = (35);	    
	LCDRAM = (2);
	LCDRAM = (0x0004);
	LCDREG = (0x00E0);  // PLL enable
	LCDRAM = (0x0001);
	DelayMs(5);
	LCDREG = (0x00E0);
	LCDRAM = (0x0003);
	DelayMs(5);
	LCDREG = (0x00E6);	//PLL setting for PCLK, depends on resolution
	LCDRAM = (0x0003);
	LCDRAM = (0x00ff);
	LCDRAM = (0x00ff);

	LCDREG = (0x00B0);	//LCD SPECIFICATION
	LCDRAM = (0x0027);
	LCDRAM = (0x0000);
	LCDRAM = ((HDP>>8)&0X00FF);  //Set HDP
	LCDRAM = (HDP&0X00FF);
    LCDRAM = ((VDP>>8)&0X00FF);  //Set VDP
	LCDRAM = (VDP&0X00FF);
    LCDRAM = (0x0000);

	LCDREG = (0x00B4);	//HSYNC
	LCDRAM = ((HT>>8)&0X00FF);  //Set HT
	LCDRAM = (HT&0X00FF);
	LCDRAM = ((HPS>>8)&0X00FF);  //Set HPS
	LCDRAM = (HPS&0X00FF);
	LCDRAM = (HPW);			   //Set HPW
	LCDRAM = ((LPS>>8)&0X00FF);  //Set HPS
	LCDRAM = (LPS&0X00FF);
	LCDRAM = (0x0000);

	LCDREG = (0x00B6);	//VSYNC
	LCDRAM = ((VT>>8)&0X00FF);   //Set VT
	LCDRAM = (VT&0X00FF);
	LCDRAM = ((VPS>>8)&0X00FF);  //Set VPS
	LCDRAM = (VPS&0X00FF);
	LCDRAM = (VPW);			   //Set VPW
	LCDRAM = ((FPS>>8)&0X00FF);  //Set FPS
	LCDRAM = (FPS&0X00FF);

	LCDREG = (0x00BA);
	LCDRAM = (0x000F);    //GPIO[3:0] out 1

	LCDREG = (0x00B8);
	LCDRAM = (0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCDRAM = (0x0001);    //GPIO0 normal

	LCDREG = (0x0036); //rotation
	LCDRAM = (0x0000);

	LCDREG = (0x00F0); //pixel data interface
	LCDRAM = (0x0003);


	DelayMs(5);

	LCDREG = (0x0029); //display on
	//LCDREG = (0x0028); 

	LCDREG = (0x00d0); 
	LCDRAM = (0x000d);

    DelayMs(5);
#else if(LCD_TYPE == 2)// 7英寸屏
	//software reset  
	WriteCommand(0x01);
	DelayMs(10);
	WriteCommand(0x01);
	DelayMs(10);
	//set pll MN
	WriteCommand(0xe2);
	LCD_WriteRam(0x29);
	LCD_WriteRam(0x03);
	LCD_WriteRam(0x04);

	//set pll
	WriteCommand(0xe0);
	LCD_WriteRam(0x01);
	DelayMs(50);
	WriteCommand(0xe0);
	LCD_WriteRam(0x03);
	LCD_WriteRam(0x01);
	DelayMs(10);


	//Set LSHIFT Frequency    
	WriteCommand(0xe6);
	LCD_WriteRam(0x05);
	LCD_WriteRam(0x11);
	LCD_WriteRam(0x9b);

	//Set LCD Mode
	WriteCommand(0xb0);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x03);
	LCD_WriteRam(0x1f);
	LCD_WriteRam(0x01);
	LCD_WriteRam(0xdf);
	LCD_WriteRam(0x00);

	//Set Horizontal Period
	WriteCommand(0xb4);
	LCD_WriteRam(0x04);
	LCD_WriteRam(0x1f);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x2e);
	LCD_WriteRam(0x13);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);

	//Set Vertical Period
	WriteCommand(0xb6);
	LCD_WriteRam(0x02);
	LCD_WriteRam(0x0c);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x17);
	LCD_WriteRam(0x09);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);

	//Set Pixel Data Interface
	WriteCommand(0xf0); 
	LCD_WriteRam(0x03);  //0x00 for 8 bit,0x02 for 16 bit

	//Set PWM Configuration
	WriteCommand(0xbe);
	LCD_WriteRam(0xff);
	LCD_WriteRam(0xff);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);

	//Set DBC Threshold
	WriteCommand(0xd4);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x1C);
	LCD_WriteRam(0x20);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x46);
	LCD_WriteRam(0x50);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0xA8);
	LCD_WriteRam(0xC0);

	//Set DBC Configuration
	WriteCommand(0xd0);
	LCD_WriteRam(0x00);

	//Exit Sleep Mode
	WriteCommand(0x11);
	DelayMs(10);

	//Set Column Address
	WriteCommand(0x2a);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x03);
	LCD_WriteRam(0x1f);

	//Set Page Address
	WriteCommand(0x2b);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x00);
	LCD_WriteRam(0x01);
	LCD_WriteRam(0xdf);	
	
	//Write Memory Start
	WriteCommand(0x2c);

	//Set Display On
	WriteCommand(0x29);
#endif
	LCD_BackLightInit();
}
void SSD1963_SetCursor(u16 Xpos, u16 Ypos)
{
	LCDREG = (0x002A);
	LCDRAM = (Xpos>>8);	    
	LCDRAM = (Xpos&0x00ff);
	LCDRAM = (Xpos>>8);	    
	LCDRAM = (Xpos&0x00ff);
    LCDREG = (0x002b);
	LCDRAM = (Ypos>>8);	    
	LCDRAM = (Ypos&0x00ff);
	LCDRAM = (Ypos>>8);	    
	LCDRAM = (Ypos&0x00ff);
}
void SSD1963_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend)
{
	LCDREG = (0x002A);
	LCDRAM = (Xpos>>8);	    
	LCDRAM = (Xpos&0x00ff);
	LCDRAM = (Xend>>8);	    
	LCDRAM = (Xend&0x00ff);
    LCDREG = (0x002b);
	LCDRAM = (Ypos>>8);	    
	LCDRAM = (Ypos&0x00ff);
	LCDRAM = (Yend>>8);	    
	LCDRAM = (Yend&0x00ff);
}
void SSD1963_SetPoint(u16 x,u16 y,u16 point)
{
	LCDREG = (0x002A);
	LCDRAM = (x>>8);	    
	LCDRAM = (x&0x00ff);
	LCDRAM = (x>>8);	    
	LCDRAM = (x&0x00ff);
    LCDREG = (0x002b);
	LCDRAM = (y>>8);	    
	LCDRAM = (y&0x00ff);
	LCDRAM = (y>>8);	    
	LCDRAM = (y&0x00ff);
	LCDREG = (0x002c);
	LCDRAM = (point);
}
u16  SSD1963_GetPoint(u16 x,u16 y)
{
	u16 d;
	u8 i;
	LCDREG = (0x002A);
	LCDRAM = (x>>8);	    
	LCDRAM = (x&0x00ff);
	LCDRAM = ((XSIZE-1)>>8);	    
	LCDRAM = ((XSIZE-1)&0x00ff);
    LCDREG = (0x002b);
	LCDRAM = (y>>8);	    
	LCDRAM = (y&0x00ff);
	LCDRAM = ((YSIZE-1)>>8);	    
	LCDRAM = ((YSIZE-1)&0x00ff);
	LCDREG = (0x002e);
	d=LCDRAM;
	return 	d;


}

void SSD1963_SetScanMode(u8 xyscan)
{
	u8 x;
	x=0;
	if(xyscan&0x10)
	{
		x |= 0x40;//水平翻转
	}
	if(xyscan&0x01)
	{
		x |= 0x80;//竖直翻转
	}
	LCDREG = (0x0036);
	LCDRAM = (x);	 
}
