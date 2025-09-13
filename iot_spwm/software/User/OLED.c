
#include "stm32f10x.h"
#include "OLED.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "IO.h"

/**
  * 数据存储格式：
  * 纵向8点，高位在下，先从左到右，再从上到下
  * 每一个Bit对应一个像素点
  *
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  *
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  *
  *       0             X轴           127
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  *
  */


/*全局变量*********************/

/**
  * OLED显存数组
  * 所有的显示函数，都只是对此显存数组进行读写
  * 随后调用OLED_Update函数或OLED_UpdateArea函数
  * 才会将显存数组的数据发送到OLED硬件，进行显示
  */
#define OLED_SCL_PORT GPIOB
#define OLED_SCL_PIN 8

#define OLED_SDA_PORT GPIOB
#define OLED_SDA_PIN 9
uint8_t OLED_DisplayBuf[8][128];

void OLED_W_SCL(uint8_t BitValue)
{
    /*根据BitValue的值，将SCL置高电平或者低电平*/
    if(BitValue) {
        SET_IO_HIGH(OLED_SCL_PORT, OLED_SCL_PIN);
    } else {
        SET_IO_LOW(OLED_SCL_PORT, OLED_SCL_PIN);
    }

}

/**
  * 函    数：OLED写SDA高低电平
  * 参    数：要写入SDA的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SDA时，此函数会被调用
  *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
  *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
  */
void OLED_W_SDA(uint8_t BitValue)
{
    if(BitValue) {
        SET_IO_HIGH(OLED_SDA_PORT, OLED_SDA_PIN);
    } else {
        SET_IO_LOW(OLED_SDA_PORT, OLED_SDA_PIN);
    }
}

/**
  * 函    数：OLED引脚初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：当上层函数需要初始化时，此函数会被调用
  *           用户需要将SCL和SDA引脚初始化为开漏模式，并释放引脚
  */
void OLED_GPIO_Init(void)
{
    uint32_t i, j;


    SET_IO_HIGH(OLED_SCL_PORT, OLED_SCL_PIN);
    SET_IO_HIGH(OLED_SDA_PORT, OLED_SDA_PIN);
    /*释放SCL和SDA*/
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/*********************引脚配置*/


/*通信协议*********************/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);		//释放SDA，确保SDA为高电平
    OLED_W_SCL(1);		//释放SCL，确保SCL为高电平
    OLED_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
    OLED_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);		//拉低SDA，确保SDA为低电平
    OLED_W_SCL(1);		//释放SCL，使SCL呈现高电平
    OLED_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;

    /*循环8次，主机依次发送数据的每一位*/
    for (i = 0; i < 8; i++)
    {
        /*使用掩码的方式取出Byte的指定一位数据并写入到SDA线*/
        /*两个!的作用是，让所有非零的值变为1*/
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
        OLED_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
    }

    OLED_W_SCL(1);		//额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}

/**
  * 函    数：OLED写命令
  * 参    数：Command 要写入的命令值，范围：0x00~0xFF
  * 返 回 值：无
  */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();				//I2C起始
    OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
    OLED_I2C_SendByte(0x00);		//控制字节，给0x00，表示即将写命令
    OLED_I2C_SendByte(Command);		//写入指定的命令
    OLED_I2C_Stop();				//I2C终止
}

/**
  * 函    数：OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
    uint8_t i;

    OLED_I2C_Start();				//I2C起始
    OLED_I2C_SendByte(0x78);		//发送OLED的I2C从机地址
    OLED_I2C_SendByte(0x40);		//控制字节，给0x40，表示即将写数据
    /*循环Count次，进行连续的数据写入*/
    for (i = 0; i < Count; i ++)
    {
        OLED_I2C_SendByte(Data[i]);	//依次发送Data的每一个数据
    }
    OLED_I2C_Stop();				//I2C终止
}

/*********************通信协议*/


/*硬件配置*********************/

/**
  * 函    数：OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void OLED_Init(void)
{

    OLED_GPIO_Init();			//先调用底层的端口初始化

    /*写入一系列的命令，对OLED进行初始化配置*/
    OLED_WriteCommand(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启

    OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);	//0x00~0xFF

    OLED_WriteCommand(0xA8);	//设置多路复用率
    OLED_WriteCommand(0x3F);	//0x0E~0x3F

    OLED_WriteCommand(0xD3);	//设置显示偏移
    OLED_WriteCommand(0x00);	//0x00~0x7F

    OLED_WriteCommand(0x40);	//设置显示开始行，0x40~0x7F

    OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置

    OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

    OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);	//设置对比度
    OLED_WriteCommand(0xCF);	//0x00~0xFF

    OLED_WriteCommand(0xD9);	//设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

    OLED_WriteCommand(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

    OLED_WriteCommand(0x8D);	//设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);	//开启显示

    OLED_Clear();				//清空显存数组
    OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}

/**
  * 函    数：OLED设置显示光标位置
  * 参    数：Page 指定光标所在的页，范围：0~7
  * 参    数：X 指定光标所在的X轴坐标，范围：0~127
  * 返 回 值：无
  * 说    明：OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
  */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
    /*通过指令设置页地址和列地址*/
    OLED_WriteCommand(0xB0 | Page);					//设置页位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;	//结果默认为1
    while (Y --)			//累乘Y次
    {
        Result *= X;		//每次把X累乘到结果上
    }
    return Result;
}
void OLED_Update(void)
{
    uint8_t j;
    /*遍历每一页*/
    for (j = 0; j < 8; j ++)
    {
        /*设置光标位置为每一页的第一列*/
        OLED_SetCursor(j, 0);
        /*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
        OLED_WriteData(OLED_DisplayBuf[j], 128);
    }
}
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
    uint8_t i, j;

    /*参数检查，保证指定区域不会超出屏幕范围*/
    if (X > 127) {
        return;
    }
    if (Y > 63) {
        return;
    }
    if (X + Width > 128) {
        Width = 128 - X;
    }
    if (Y + Height > 64) {
        Height = 64 - Y;
    }

    for (j = Y; j < Y + Height; j ++)		//遍历指定页
    {
        for (i = X; i < X + Width; i ++)	//遍历指定列
        {
            OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
        }
    }
}

void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{

}

void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j ++)				//遍历8页
    {
        for (i = 0; i < 128; i ++)			//遍历128列
        {
            OLED_DisplayBuf[j][i] = 0x00;	//将显存数组数据全部清零
        }
    }
}


void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
    if (FontSize == OLED_8X16)		//字体为宽8像素，高16像素
    {
        /*将ASCII字模库OLED_F8x16的指定数据以8*16的图像格式显示*/
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    }
    else if(FontSize == OLED_6X8)	//字体为宽6像素，高8像素
    {
        /*将ASCII字模库OLED_F6x8的指定数据以6*8的图像格式显示*/
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
    }
}

/**
  * 函    数：OLED显示字符串
  * 参    数：X 指定字符串左上角的横坐标，范围：0~127
  * 参    数：Y 指定字符串左上角的纵坐标，范围：0~63
  * 参    数：String 指定要显示的字符串，范围：ASCII码可见字符组成的字符串
  * 参    数：FontSize 指定字体大小
  *           范围：OLED_8X16		宽8像素，高16像素
  *                 OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)		//遍历字符串的每个字符
    {
        /*调用OLED_ShowChar函数，依次显示每个字符*/
        OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
    }
}

void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++)		//遍历数字的每一位
    {
        /*调用OLED_ShowChar函数，依次显示每个数字*/
        /*Number / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
        /*+ '0' 可将数字转换为字符格式*/
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}

void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    uint8_t i, j;

    /*参数检查，保证指定图像不会超出屏幕范围*/
    if (X > 127) {
        return;
    }
    if (Y > 63) {
        return;
    }

    /*将图像所在区域清空*/
    OLED_ClearArea(X, Y, Width, Height);

    /*遍历指定图像涉及的相关页*/
    /*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
    for (j = 0; j < (Height - 1) / 8 + 1; j ++)
    {
        /*遍历指定图像涉及的相关列*/
        for (i = 0; i < Width; i ++)
        {
            /*超出边界，则跳过显示*/
            if (X + i > 127) {
                break;
            }
            if (Y / 8 + j > 7) {
                return;
            }

            /*显示图像在当前页的内容*/
            OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);

            /*超出边界，则跳过显示*/
            /*使用continue的目的是，下一页超出边界时，上一页的后续内容还需要继续显示*/
            if (Y / 8 + j + 1 > 7) {
                continue;
            }

            /*显示图像在下一页的内容*/
            OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
        }
    }
}
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
    uint8_t pChinese = 0;
    uint8_t pIndex;
    uint8_t i;
    char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};

    for (i = 0; Chinese[i] != '\0'; i ++)		//遍历汉字串
    {
        SingleChinese[pChinese] = Chinese[i];	//提取汉字串数据到单个汉字数组
        pChinese ++;							//计次自增

        /*当提取次数到达OLED_CHN_CHAR_WIDTH时，即代表提取到了一个完整的汉字*/
        if (pChinese >= OLED_CHN_CHAR_WIDTH)
        {
            pChinese = 0;		//计次归零

            /*遍历整个汉字字模库，寻找匹配的汉字*/
            /*如果找到最后一个汉字（定义为空字符串），则表示汉字未在字模库定义，停止寻找*/
            for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
            {
                /*找到匹配的汉字*/
                if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
                {
                    break;		//跳出循环，此时pIndex的值为指定汉字的索引
                }
            }

            /*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
            OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
        }
    }
}
