/*
 * File: App.c
Author: {zhangzhy108@163.com, WX&QQ:1076279743}
Copyright (c) 2025 {zhangzhy108@163.com}
 */


#include "App.h"
#include "IO.h"
#include "Timer.h"
#include "switch.h"
#include "spwm.h"
#include "adc.h"
#include "OLED.h"
#include "uart.h"
#include "DMA.h"
STRAPType g_ap_stRegs;
void fnAP_Init(void);
void fnAP_RealTime(void);

void fnAP_Init(void) {
    memset(&g_ap_stRegs, 0, sizeof(g_ap_stRegs));
    SET_IO_LOW(P_LED_RED_PORT, P_LED_RED_PIN);
    OLED_Init();

}
#define APP_STATE_IDLE 0
#define APP_STATE_RUN 1
#define SCR_MAX_DUTY 10000
#define SCR_MIN_DUTY 10
const U16 decdiv[] = {1000, 100, 10, 1};
int int_to_str(int num, char *str) {
    if (num == 0) {  // 处理 0 的特殊情况
        str[0] = '0';
        str[1] = '\0';
        return 1;
    }

    int is_negative = 0;
    if (num < 0) {   // 处理负数
        is_negative = 1;
        num = -num;  // 转为正数处理（注意 INT_MIN 会溢出）
    }

    // 逐位提取数字（反向）
    char temp[12];
    int index = 0;
    while (num > 0) {
        temp[index++] = (num % 10) + '0';
        num /= 10;
    }

    // 反转并添加符号
    int str_index = 0;
    if (is_negative) {
        str[str_index++] = '-';
    }
    for (int i = index - 1; i >= 0; i--) {
        str[str_index++] = temp[i];
    }
    str[str_index] = '\0';
    return(str_index);
}
void fnAP_Display(void) {
    U32 data;
    char valchars[20];
    int charlen;
    char dischars[20];
    U8 i;
    if(g_tm_stTimerFlag.Bits.bTimer100ms) {
        if(g_ap_stRegs.state == APP_STATE_IDLE) {
            OLED_ShowChinese(0,0,"停止");
        } else {
            OLED_ShowChinese(0,0,"启动");
        }
        if(g_ap_stRegs.remotecontrol) {
            OLED_ShowChinese(60,0,"远程");
        } else {
            OLED_ShowChinese(60,0,"本地");
        }

        OLED_ShowChinese(0,16,"电压：");
        charlen = int_to_str(g_ap_stRegs.voltage / 10, valchars);//g_ap_stRegs.voltage;
        for(i = 0; i < 10; i++) {
            if(i < charlen) {
                dischars[i] = valchars[i];
            } else {
                dischars[i] = ' ';
            }
        }
        dischars[10] = 0;
        OLED_ShowString(64, 16, dischars, 8);
        ///OLED_ShowNum(64,16,g_ap_stRegs.voltage,5,OLED_8X16);

        OLED_ShowChinese(0,32,"调制深度：");
        charlen = int_to_str(g_ap_stRegs.duty, valchars);
        for(i = 0; i < 10; i++) {
            if(i <= charlen) {
                dischars[i] = valchars[i];
            } else {
                dischars[i] = ' ';
            }
        }
        dischars[10] = 0;
        OLED_ShowString(80, 32, dischars, 8);
        //OLED_ShowNum(80,32,g_ap_stRegs.duty,5,OLED_8X16);
        OLED_Update();
    }
}
#define APP_UART_CMD_SETREMOTE 1
#define APP_UART_CMD_SETDUFY 2
#define APP_UART_CMD_SETCON 3
#define APP_UART_CMD_REPORT 4

//55 FF 00 01 00 02 01 00  55 结束远程控制
//55 FF 00 01 00 02 01 01  55 启动远程控制

//55 FF 00 01 00 03 02 xx xx  55 启动远程控制之后，调节调制深度 0-9999

//55 FF 00 01 00 02 03 01  55 启动输出
//55 FF 00 01 00 02 03 00  55 停止输出

//ad 736->1290
void fnAP_RealTime(void) {
    U16 duty;
    U32 data;
    U16 setduty;
    U8 uartbuff[16];
    U8 changestate;
    if(g_tm_stTimerFlag.Bits.bTimer100ms) {
        data = (U32)g_ad_stRegs.m_uiADFilterValue[AD_CHANNEL_OFF_U1];
        data = data * (U32)1290;
        data = data / 748;
        g_ap_stRegs.voltage = (U16)data;
    }
    if(g_ap_stRegs.state == APP_STATE_IDLE) {
        changestate = FALSE;
        if(SW_START_EVENT()) {
            changestate = TRUE;

        }
        if(g_ap_stRegs.remotecontrol) {
            if(g_ap_stRegs.remotestart) {
                changestate = TRUE;
            }
        }
        if(changestate) {
            g_ap_stRegs.duty = 0;
            fnSPWM_EnableIPWM();
            g_ap_stRegs.state = APP_STATE_RUN;
        }

    } else {
        changestate = FALSE;
        if(SW_START_EVENT()) {
            changestate = TRUE;

        }
        if(g_ap_stRegs.remotecontrol) {
            if(g_ap_stRegs.remotestart == FALSE) {
                changestate = TRUE;
            }
        }
        if(changestate) {
            g_ap_stRegs.duty = 0;
            fnSPWM_DisableIPWM();
            g_ap_stRegs.state = APP_STATE_IDLE;

        } else {
            if(g_tm_stTimerFlag.Bits.bTimer100ms) {
                if(g_ap_stRegs.remotecontrol == FALSE) {
                    if(g_ad_stRegs.m_uchReady[AD_CHANNEL_OFF_VOLSET]) {
                        data = g_ad_stRegs.m_uiADFilterValue[AD_CHANNEL_OFF_VOLSET];
                        data = data * (SCR_MAX_DUTY);
                        duty = (U32)data/4096;

                        if(duty > SCR_MAX_DUTY)
                        {
                            duty = SCR_MAX_DUTY;
                        }
                        if(duty < SCR_MIN_DUTY)
                        {
                            duty = SCR_MIN_DUTY;
                        }
                        g_ap_stRegs.duty = duty;
                    }
                }
                if(g_ap_stRegs.duty > SCR_MAX_DUTY)
                {
                    g_ap_stRegs.duty = SCR_MAX_DUTY;
                }
                if(g_ap_stRegs.duty < SCR_MIN_DUTY)
                {
                    g_ap_stRegs.duty = SCR_MIN_DUTY;
                }
                fnSPWM_UpdateDuty(g_ap_stRegs.duty);
            }

        }
    }
    if(g_ap_stRegs.state == APP_STATE_IDLE) {
        if(g_tm_stTimerFlag.Bits.bTimer1s) {
            SET_IO_TOGGLE(P_LED_RED_PORT, P_LED_RED_PIN);
        }
    }
    else {
        if(g_tm_stTimerFlag.Bits.bTimer100ms) {
            SET_IO_TOGGLE(P_LED_RED_PORT, P_LED_RED_PIN);
        }
    }
    fnAP_Display();

    if(g_ua_stRecRegs.m_uchReceived) {
        if(APP_UART_CMD_SETREMOTE == g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA]) {
            if(g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA + 1]) {
                g_ap_stRegs.remotecontrol = TRUE;
                g_ap_stRegs.remotestart = FALSE;
            } else {
                g_ap_stRegs.remotecontrol = FALSE;
                g_ap_stRegs.remotestart = FALSE;
            }

        }
        if(APP_UART_CMD_SETCON == g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA]) {
            if(g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA + 1]) {
                g_ap_stRegs.remotestart = TRUE;
            } else {
                g_ap_stRegs.remotestart = FALSE;
            }

        }
        if(APP_UART_CMD_SETDUFY == g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA]) {
            setduty = (U16)g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA + 1];
            setduty = setduty << 8;
            setduty |= (U16)g_ua_stRecRegs.m_uchBuff[UA_MSG_OFFSET_DATA + 2];
            if(setduty < SCR_MAX_DUTY) {
                g_ap_stRegs.duty = setduty;
            }
        }
        g_ua_stRecRegs.m_uchReceived = FALSE;
    }
    if(g_tm_stTimerFlag.Bits.bTimer100ms) {
        uartbuff[0] = APP_UART_CMD_REPORT;
        uartbuff[1] = g_ap_stRegs.remotecontrol;
        uartbuff[2] = g_ap_stRegs.state;
        uartbuff[3] = (g_ap_stRegs.duty >> 8) & 0xff;
        uartbuff[4] = (g_ap_stRegs.duty & 0xff);
        uartbuff[5] = (g_ap_stRegs.voltage >> 8) & 0xff;
        uartbuff[6] = (g_ap_stRegs.voltage & 0xff);
        fnDMA_UARTTxStart(uartbuff, 7);
    }

}
/*
55 FF 00 01 00 02 01 00 55 //本地控制
55 FF 00 01 00 02 01 01 55 //远程控制
55 FF 00 01 00 02 03 01 55 //启动
55 FF 00 01 00 02 03 00 55 //停止
55 FF 00 01 00 03 02 xx yy 55 //调制深度 xxyy=0-9999，需转成16进制
*/

