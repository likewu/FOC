#ifndef _H_DEFINE_
#define _H_DEFINE_
#include "string.h"
#include "stm32f10x.h"
#define SYSCLK_SYS_FREQ 72000000
#define CPU_FREQ_MHZ 150
#define CPU_FREQ_LSPCK  37.5
#define CPU_FREQ_HZ (U32)150000000

#define PLC_ID_COMCODE_H 0xAC
#define PLC_ID_COMCODE_L 0x00
#define PLC_TYPE_ZVC 0x32
#define PLC_TYPE PLC_TYPE_ZVC
#define AD_EXT_NUM 4
#define XNUM  14
#define YNUM 4



#define HIGH 1
#define LOW 0
#define OK 1
#define NG 0
#define TRUE 1
#define FALSE 0
#define TEST 0
#define VERSION 0x304

typedef uint16_t U16;
typedef uint8_t U8;
typedef uint32_t U32;
typedef signed int INT16;
typedef long long U64;
#define DATEYEAR        0x13
#define DATEMONTH       0x10
#define DATEDAY         0x09
#define ROUTINE_NUM 24
#define ROUTINE_STEP_NUM 64
#define _MODBUS_ TRUE
#define YW_PT100 FALSE

#define WF_SSID_LEN 32
#define WF_PWD_LEN 32

#define AP_ID_LEN 26

#define AP_ID_HEX_BYTE 12

#define AP_SHORTID_HEX_BYTE 5
#define WF_CONAP_LEN WF_SSID_LEN
#define WF_CONPWD_LEN WF_PWD_LEN

#define AP_HOST_LEN 32

#define AP_HOSTPWD_LEN 32

#define AP_NAME_LEN 12

#define PLC_READPROTECTS 0

#define SWVER 0x524
#define HWVER 0x400

#define SOFTWARE_MAGICNUM_NUM 11
#define SOFTWARE_MAGICNUM_OFF_HEADER 0
#define SOFTWARE_MAGICNUM_OFF_DEVTYPE 4

#define SOFTWARE_MAGICNUM_OFF_SVERH 5
#define SOFTWARE_MAGICNUM_OFF_SVERL 6
#define SOFTWARE_MAGICNUM_OFF_HVERH 7
#define SOFTWARE_MAGICNUM_OFF_HVERL 8
#define SOFTWARE_MAGICNUM_OFF_XNUM 9
#define SOFTWARE_MAGICNUM_OFF_YNUM 10
extern const U8 g_mn_uchSoftwareMagicNum[SOFTWARE_MAGICNUM_NUM];
extern void fnMN_MAPR_OR(U32 value);
extern void fnMN_MAPR_And(U32 value);
extern void fnMN_MAPR_Set(void);
extern U8 g_mn_U8Position[];
extern U16 g_mn_U16Position[];
extern void fnMN_DelayMs(U32 nCount);
extern void fnMN_Timer1s(void);
#endif
