
#include "stdio.h"
#include "math.h"
#include "define.h"
#include "Timer.h"
#include "IO.h"
#include "App.h"
#include "Switch.h"
#include "ADC.h"
#include "uart.h"
#include "spwm.h"
U8 g_mn_U8Position[]=
{
    128, 64, 32, 16, 8, 4,2,1
};
U16 g_mn_U16Position[]=
{
    1,2,4,8,16,32,64,128,256,512, 1024, 2048, 4096, 8192, 16384, 32768,
};
/*----------------------- Function Implement --------------------------------*/

const U8 g_mn_uchSoftwareMagicNum[SOFTWARE_MAGICNUM_NUM] =
{
    'Z','Z','Y', 'S',
    PLC_TYPE, (U8)(SWVER >> 8), (U8)SWVER,
    (U8)(HWVER >> 8), (U8)HWVER, XNUM, YNUM
};

U32 mn_mapr;
void fnMN_MAPR_OR(U32 value)
{
    mn_mapr |= value;
}
void fnMN_MAPR_And(U32 value)
{
    mn_mapr &= value;
}
void fnMN_MAPR_Set(void)
{
    AFIO->MAPR = mn_mapr;
}
void fnMN_IOInit(void)
{
    mn_mapr = 0;
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN
                     | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN
                     | RCC_APB2ENR_IOPEEN | RCC_APB2ENR_IOPEEN
                     | RCC_APB2ENR_AFIOEN);//PORTA clock enable

    RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_SPI1EN);
    RCC->APB1ENR |=  (RCC_APB1ENR_USART3EN| RCC_APB1ENR_USART2EN);
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    fnMN_MAPR_OR(AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
    fnMN_MAPR_OR(AFIO_MAPR_USART1_REMAP);
    fnMN_MAPR_Set();
}


void fnMN_DelayMs(U32 nCount)
{
    U32 i, j;
    for(i = 0; i< nCount; i++)
    {
        for(j = 0; j< 3; j++)
        {
        }
    }
}



void fnMN_IOTClear(void) {

}
void fnMN_WTInit(void)
{
    IWDG->KR = 0x5555;
    IWDG->PR = 4;
    IWDG->RLR = 625;    //1s
    IWDG->KR = 0xAAAA;
    IWDG->KR = 0xCCCC;
}

void fnMN_ReadProtect(void)
{


}

int main(void)
{


    fnMN_DelayMs(20000);
    fnMN_IOInit();
    fnTM_Init();
    fnSW_Init();
    fnAD_Init();

    fnUA_Init();
    fnAP_Init();
    //SET_IO_HIGH(GPIOA, 9);
    fnSPWM_Init();

    //fnMN_WTInit();
    while(1)	{

        fnTM_RealTime();
        fnSW_RealTime();
        fnAD_RealTime();
        fnUA_RealTime();
        fnSPWM_RealTime();

        fnAP_RealTime();
        fnAD_Clear();
        fnSW_Clear();
        fnTM_Clear();
        //IWDG->KR = 0xAAAA;
    }
    return 0;
}


