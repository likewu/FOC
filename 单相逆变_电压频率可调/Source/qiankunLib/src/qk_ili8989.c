#include "qk_ILI8989.h"

static u16 DeviceCode = 0x8989;

void ILI8989_Init(void)
{
	u16 i;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 	
	//配置LCD使用的GPIO
	LCD_InitGPIO();
	//配置FSMC接口
	LCD_FSMCConfig();

	DelayMs(5);
	//初始化液晶
	if(DeviceCode==0x9325||DeviceCode==0x9328)
	{
	    LCD_WriteReg(0x00e7,0x0010);      
	    LCD_WriteReg(0x0000,0x0001);  			//start internal osc
	    LCD_WriteReg(0x0001,0x0100);     
	    LCD_WriteReg(0x0002,0x0700); 		    //power on sequence                     
	    LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4) ); 	//65K 
	    LCD_WriteReg(0x0004,0x0000);                                   
	    LCD_WriteReg(0x0008,0x0207);	           
	    LCD_WriteReg(0x0009,0x0000);         
	    LCD_WriteReg(0x000a,0x0000); 			//display setting         
	    LCD_WriteReg(0x000c,0x0001);			//display setting          
	    LCD_WriteReg(0x000d,0x0000); 			//0f3c          
	    LCD_WriteReg(0x000f,0x0000);
	
	    //Power On sequence //
	    LCD_WriteReg(0x0010,0x0000);   
	    LCD_WriteReg(0x0011,0x0007);
	    LCD_WriteReg(0x0012,0x0000);                                                                 
	    LCD_WriteReg(0x0013,0x0000);                 
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0010,0x1590);   
	    LCD_WriteReg(0x0011,0x0227);
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0012,0x009c);                  
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0013,0x1900);   
	    LCD_WriteReg(0x0029,0x0023);
	    LCD_WriteReg(0x002b,0x000e);
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0020,0x0000);                                                            
	    LCD_WriteReg(0x0021,0x0000);           
	///////////////////////////////////////////////////////      
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0030,0x0007); 
	    LCD_WriteReg(0x0031,0x0707);   
	    LCD_WriteReg(0x0032,0x0006);
	    LCD_WriteReg(0x0035,0x0704);
	    LCD_WriteReg(0x0036,0x1f04); 
	    LCD_WriteReg(0x0037,0x0004);
	    LCD_WriteReg(0x0038,0x0000);        
	    LCD_WriteReg(0x0039,0x0706);     
	    LCD_WriteReg(0x003c,0x0701);
	    LCD_WriteReg(0x003d,0x000f);
	    for(i=50000;i>0;i--);
	      for(i=50000;i>0;i--);
	    LCD_WriteReg(0x0050,0x0000);        
	    LCD_WriteReg(0x0051,0x00ef);   
	    LCD_WriteReg(0x0052,0x0000);     
	    LCD_WriteReg(0x0053,0x013f);
	    LCD_WriteReg(0x0060,0xa700);        
	    LCD_WriteReg(0x0061,0x0001); 
	    LCD_WriteReg(0x006a,0x0000);
	    LCD_WriteReg(0x0080,0x0000);
	    LCD_WriteReg(0x0081,0x0000);
	    LCD_WriteReg(0x0082,0x0000);
	    LCD_WriteReg(0x0083,0x0000);
	    LCD_WriteReg(0x0084,0x0000);
	    LCD_WriteReg(0x0085,0x0000);
	  
	    LCD_WriteReg(0x0090,0x0010);     
	    LCD_WriteReg(0x0092,0x0000);  
	    LCD_WriteReg(0x0093,0x0003);
	    LCD_WriteReg(0x0095,0x0110);
	    LCD_WriteReg(0x0097,0x0000);        
	    LCD_WriteReg(0x0098,0x0000);  
	    //display on sequence     
	    LCD_WriteReg(0x0007,0x0133);
	
	    LCD_WriteReg(0x0020,0x0000);                                                            
	    LCD_WriteReg(0x0021,0x0000);
	}
	else if(DeviceCode==0x9320||DeviceCode==0x9300)
	{
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
	}
	else if(DeviceCode==0x9331)
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit   0x0100
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
		LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.     0x1030
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		DelayMs(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		DelayMs(50); // DelayMs 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		DelayMs(50); // DelayMs 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		DelayMs(50); // DelayMs 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x0000); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);		
		DelayMs(50);
		LCD_WriteReg(0x0007,0x0061);
		DelayMs(50);
		LCD_WriteReg(0x0007,0x0133);  // 262K color and display ON
		DelayMs(50);
	}
	else if(DeviceCode==0x9919)
	{
		 //*********POWER ON &RESET DISPLAY OFF
		 LCD_WriteReg(0x28,0x0006);
		
		 LCD_WriteReg(0x00,0x0001);
		
		 LCD_WriteReg(0x10,0x0000);
		
		 LCD_WriteReg(0x01,0x72ef);
	
		 LCD_WriteReg(0x02,0x0600);
	
		 LCD_WriteReg(0x03,0x6a38);
		
		 LCD_WriteReg(0x11,0x6874);//70
		
		 
		 //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0f,0x0000); 
		 //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0b,0x5308); 
		
		 LCD_WriteReg(0x0c,0x0003);
		
		 LCD_WriteReg(0x0d,0x000a);
		
		 LCD_WriteReg(0x0e,0x2e00);  //0030
		
		 LCD_WriteReg(0x1e,0x00be);
		
		 LCD_WriteReg(0x25,0x8000);
		
		 LCD_WriteReg(0x26,0x7800);
		
		 LCD_WriteReg(0x27,0x0078);
		
		 LCD_WriteReg(0x4e,0x0000);
		
		 LCD_WriteReg(0x4f,0x0000);
		
		 LCD_WriteReg(0x12,0x08d9);
		
		 // -----------------Adjust the Gamma Curve----//
		 LCD_WriteReg(0x30,0x0000);	 //0007
		
		 LCD_WriteReg(0x31,0x0104);	 //0203
		
		 LCD_WriteReg(0x32,0x0100);  //0001
	
		 LCD_WriteReg(0x33,0x0305);	 //0007
	
		 LCD_WriteReg(0x34,0x0505);	 //0007
		
		 LCD_WriteReg(0x35,0x0305);	 //0407
		
		 LCD_WriteReg(0x36,0x0707);	 //0407
		
		 LCD_WriteReg(0x37,0x0300);	 //0607
		
		 LCD_WriteReg(0x3a,0x1200);	 //0106
		
		 LCD_WriteReg(0x3b,0x0800);		 
	
		 LCD_WriteReg(0x07,0x0033);
	}
	else if(DeviceCode==0x1505)
	{
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
	}							 
	else if(DeviceCode==0x8989)
	{
	    LCD_WriteReg(0x0000,0x0001);       //打开晶振
    	LCD_WriteReg(0x0003,0xA8A4);       //0xA8A4
    	LCD_WriteReg(0x000C,0x0000);       
    	LCD_WriteReg(0x000D,0x080C);       
    	LCD_WriteReg(0x000E,0x2B00);       
    	LCD_WriteReg(0x001E,0x00B0);       
    	LCD_WriteReg(0x0001,0x2B3F);       //驱动输出控制320*240  0x6B3F
    	LCD_WriteReg(0x0002,0x0600);    
    	LCD_WriteReg(0x0010,0x0000);    
    	LCD_WriteReg(0x0011,0x6058);       //0x4030 定义数据格式  16位色 		横屏 0x6058   0x6070
    	LCD_WriteReg(0x0005,0x0000);    
    	LCD_WriteReg(0x0006,0x0000);    
    	LCD_WriteReg(0x0016,0xEF1C);    
    	LCD_WriteReg(0x0017,0x0003);    
    	LCD_WriteReg(0x0007,0x0233);       //0x0233       
    	LCD_WriteReg(0x000B,0x0000);    
    	LCD_WriteReg(0x000F,0x0000);       //扫描开始地址
    	LCD_WriteReg(0x0041,0x0000);    
    	LCD_WriteReg(0x0042,0x0000);    
    	LCD_WriteReg(0x0048,0x0000);    
    	LCD_WriteReg(0x0049,0x013F);    
    	LCD_WriteReg(0x004A,0x0000);    
    	LCD_WriteReg(0x004B,0x0000);    
    	LCD_WriteReg(0x0044,0xEF00);    
    	LCD_WriteReg(0x0045,0x0000);    
    	LCD_WriteReg(0x0046,0x013F);    
    	LCD_WriteReg(0x0030,0x0707);    
    	LCD_WriteReg(0x0031,0x0204);    
    	LCD_WriteReg(0x0032,0x0204);    
    	LCD_WriteReg(0x0033,0x0502);    
    	LCD_WriteReg(0x0034,0x0507);    
    	LCD_WriteReg(0x0035,0x0204);    
    	LCD_WriteReg(0x0036,0x0204);    
    	LCD_WriteReg(0x0037,0x0502);    
    	LCD_WriteReg(0x003A,0x0302);    
    	LCD_WriteReg(0x003B,0x0302);    
    	LCD_WriteReg(0x0023,0x0000);    
    	LCD_WriteReg(0x0024,0x0000);    
    	LCD_WriteReg(0x0025,0x8000);    
    	LCD_WriteReg(0x004f,0);        //行首址0
    	LCD_WriteReg(0x004e,0);        //列首址0
	}
	else if(DeviceCode==0x7783)
	{
		// Start Initial Sequence
		LCD_WriteReg(0x00FF,0x0001);
		LCD_WriteReg(0x00F3,0x0008);
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);  //0x1030
		LCD_WriteReg(0x0008,0x0302);
		LCD_WriteReg(0x0008,0x0207);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000);
		LCD_WriteReg(0x0010,0x0000);  //0x0790
		LCD_WriteReg(0x0011,0x0005);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		DelayMs(50);
		LCD_WriteReg(0x0010,0x12B0);
		DelayMs(50);
		LCD_WriteReg(0x0011,0x0007);
		DelayMs(50);
		LCD_WriteReg(0x0012,0x008B);
		DelayMs(50);
		LCD_WriteReg(0x0013,0x1700);
		DelayMs(50);
		LCD_WriteReg(0x0029,0x0022);
		
		//################# void Gamma_Set(void) ####################//
		LCD_WriteReg(0x0030,0x0000);
		LCD_WriteReg(0x0031,0x0707);
		LCD_WriteReg(0x0032,0x0505);
		LCD_WriteReg(0x0035,0x0107);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0000);
		LCD_WriteReg(0x0038,0x0202);
		LCD_WriteReg(0x0039,0x0106);
		LCD_WriteReg(0x003C,0x0202);
		LCD_WriteReg(0x003D,0x0408);
		DelayMs(50);
		
		LCD_WriteReg(0x0050,0x0000);		
		LCD_WriteReg(0x0051,0x00EF);		
		LCD_WriteReg(0x0052,0x0000);		
		LCD_WriteReg(0x0053,0x013F);		
		LCD_WriteReg(0x0060,0xA700);		
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x0090,0x0033);				
		LCD_WriteReg(0x002B,0x000B);		
		LCD_WriteReg(0x0007,0x0133);
		DelayMs(50);
	} 
	else if(DeviceCode==0x8999)	   //对应的驱动IC为SSD1289
	{
		//************* Start Initial Sequence **********//
		LCD_WriteReg(0x00, 0x0001); // Start internal OSC.
		LCD_WriteReg(0x01, 0x7b3F); // Driver o 0011 1101 utput control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
		LCD_WriteReg(0x02, 0x0600); // set 1 line inversion
		//************* Power control setup ************/
		LCD_WriteReg(0x0C, 0x0007); // Adjust VCIX2 output voltage
		LCD_WriteReg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
		LCD_WriteReg(0x0E, 0x3200); // Set alternating amplitude of VCOM
		LCD_WriteReg(0x1E, 0x00BB); // Set VcomH voltage
		LCD_WriteReg(0x03, 0x6A64); // Step-up factor/cycle setting
		//************ RAM position control **********/
		LCD_WriteReg(0x0F, 0x0000); // Gate scan position start at G0.
		LCD_WriteReg(0x44, 0xEF00); // Horizontal RAM address position
		LCD_WriteReg(0x45, 0x0000); // Vertical RAM address start position
		LCD_WriteReg(0x46, 0x013F); // Vertical RAM address end position
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		//************* Special command **************/
		LCD_WriteReg(0x28, 0x0006); // Enable test command
		LCD_WriteReg(0x2F, 0x12EB); // RAM speed tuning
		LCD_WriteReg(0x26, 0x7000); // Internal Bandgap strength
		LCD_WriteReg(0x20, 0xB0E3); // Internal Vcom strength
		LCD_WriteReg(0x27, 0x0044); // Internal Vcomh/VcomL timing
		LCD_WriteReg(0x2E, 0x7E45); // VCOM charge sharing time  
		//************* Turn On display ******************/
		LCD_WriteReg(0x10, 0x0000); // Sleep mode off.
		DelayMs(30); // Wait 30mS
		LCD_WriteReg(0x11, 0x6870);// Entry mode setup. 262K type B, take care on the data bus with 16it only
		LCD_WriteReg(0x07, 0x0033); // Display ON	*/
	} 
  	//先关闭TIM5
	TIM_Cmd(TIM5,DISABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//TIM5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//PA
	
	//先把IO的输出设为0，方便当PWM刹车时可以立马让PWM输出引脚变为0
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);					   
	
	//配置PA1作为PWM输出端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	//  TIM5 CH2的配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	//  PA1   PWM输出，如果有波形输出，则使能，如果只做定时器，则不使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	//  控制占空比的寄存器配置
	TIM_OCInitStructure.TIM_Pulse =100;//高电平宽度

	//  PWM输出极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Enable);

	//  TIM5的配置																			  
	TIM_TimeBaseStructure.TIM_Period=100-1;	//频率的计算：	72M/(Prescaler+1)/(Period+1)	 
	TIM_TimeBaseStructure.TIM_Prescaler=720-1; 	//预分频为0，实际计算时为0+1（都需加上1）
	TIM_TimeBaseStructure.TIM_ClockDivision=0;			//时钟分频为0，则仍为倍频后的时钟频率
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

	//打开TIM5
	TIM_Cmd(TIM5,ENABLE);
}
void ILI8989_SetCursor(u16 Xpos, u16 Ypos)
{
	if(DeviceCode==0x8989)
	{
		LCD_WriteReg(0x004e,Ypos);        //行
		LCD_WriteReg(0x004f,0x13f-Xpos);  //列
	}
	else if(DeviceCode==0x9919)
	{
		LCD_WriteReg(0x004e,Xpos);        //行
		LCD_WriteReg(0x004f,Ypos);        //列	
	} 
	else if((DeviceCode==0x8999)||(DeviceCode==0x9919))
	{
		LCD_WriteReg(0x004E, Xpos);
		LCD_WriteReg(0X004F, Ypos);
	}
	else	
	{
		LCD_WriteReg(0x0020,Ypos);        //行
		LCD_WriteReg(0x0021,0x13f-Xpos);  //列
	}
}
void ILI8989_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Xend, u16 Yend)
{
	if(DeviceCode==0x8989)
	{   
		LCD_WriteReg(0x0044,Ypos|(Yend<<8));    
		LCD_WriteReg(0x0045,319-Xend);    
		LCD_WriteReg(0x0046,319-Xpos);
	}
	else
	{
		LCD_WriteReg(0x0050,Ypos);
		LCD_WriteReg(0x0052, 319-Xend);
		LCD_WriteReg(0x0051, Yend);
		LCD_WriteReg(0x0053, 319-Xpos);
	}
	//设置光标位置
  	ILI8989_SetCursor(Xpos, Ypos);
}

void ILI8989_SetScanMode(u8 xyscan)
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
}