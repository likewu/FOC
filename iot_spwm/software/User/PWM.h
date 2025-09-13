#ifndef _H_PWM_
#define _H_PWM_
#include "define.h"
#define EPWMTypeDef struct EPWM_REGS
typedef struct EPWM_REGS *pEPWMTypeDef;

//FREE SOFT
#define FREE_RUN        0x3
// CTRMODE bits
#define	TB_COUNT_UP		0x0
#define	TB_COUNT_DOWN	0x1
#define	TB_COUNT_UPDOWN	0x2
#define	TB_FREEZE		0x3
// PHSEN bit
#define	TB_DISABLE		0x0
#define	TB_ENABLE		0x1
// PRDLD bit
#define	TB_SHADOW		0x0
#define	TB_IMMEDIATE	0x1
// SYNCOSEL bits
#define	TB_SYNC_IN		0x0
#define	TB_CTR_ZERO		0x1
#define	TB_CTR_CMPB		0x2
#define	TB_SYNC_DISABLE	0x3
// HSPCLKDIV and CLKDIV bits
#define	TB_DIV1			0x0
#define	TB_DIV2			0x1
#define	TB_DIV4			0x2
// PHSDIR bit
#define	TB_DOWN			0x0
#define	TB_UP			0x1

// CMPCTL (Compare Control)
//==========================
// LOADAMODE and LOADBMODE bits
#define	CC_CTR_ZERO		0x0
#define	CC_CTR_PRD		0x1
#define	CC_CTR_ZERO_PRD	0x2
#define	CC_LD_DISABLE	0x3
// SHDWAMODE and SHDWBMODE bits
#define	CC_SHADOW		0x0
#define	CC_IMMEDIATE	0x1

// AQCTLA and AQCTLB (Action Qualifier Control)
//=============================================
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define	AQ_NO_ACTION	0x0
#define	AQ_CLEAR		0x1
#define	AQ_SET			0x2
#define	AQ_TOGGLE		0x3

// DBCTL (Dead-Band Control)
//==========================
// OUT MODE bits
#define	DB_DISABLE		0x0
#define	DBA_ENABLE		0x1
#define	DBB_ENABLE		0x2
#define	DB_FULL_ENABLE	0x3
// POLSEL bits
#define	DB_ACTV_HI		0x0
#define	DB_ACTV_LOC		0x1
#define	DB_ACTV_HIC		0x2
#define	DB_ACTV_LO		0x3
// IN MODE
#define DBA_ALL         0x0
#define DBB_RED_DBA_FED 0x1
#define DBA_RED_DBB_FED 0x2
#define DBB_ALL         0x3

// CHPCTL (chopper control)
//==========================
// CHPEN bit
#define	CHP_DISABLE		0x0
#define	CHP_ENABLE		0x1
// CHPFREQ bits
#define	CHP_DIV1		0x0
#define	CHP_DIV2		0x1
#define	CHP_DIV3		0x2
#define	CHP_DIV4		0x3
#define	CHP_DIV5		0x4
#define	CHP_DIV6		0x5
#define	CHP_DIV7		0x6
#define	CHP_DIV8		0x7
// CHPDUTY bits
#define	CHP1_8TH		0x0
#define	CHP2_8TH		0x1
#define	CHP3_8TH		0x2
#define	CHP4_8TH		0x3
#define	CHP5_8TH		0x4
#define	CHP6_8TH		0x5
#define	CHP7_8TH		0x6

// TZSEL (Trip Zone Select)
//==========================
// CBCn and OSHTn bits
#define	TZ_DISABLE		0x0
#define	TZ_ENABLE		0x1

// TZCTL (Trip Zone Control)
//==========================
// TZA and TZB bits
#define	TZ_HIZ			0x0
#define	TZ_FORCE_HI		0x1
#define	TZ_FORCE_LO		0x2
#define	TZ_NO_CHANGE	0x3

// ETSEL (Event Trigger Select)
//=============================
#define	ET_CTR_ZERO		0x1
#define	ET_CTR_PRD		0x2
#define	ET_CTRU_CMPA	0x4
#define	ET_CTRD_CMPA	0x5
#define	ET_CTRU_CMPB	0x6
#define	ET_CTRD_CMPB	0x7

// ETPS (Event Trigger Pre-scale)
//===============================
// INTPRD, SOCAPRD, SOCBPRD bits
#define ET_ENA          0x1
#define	ET_DISABLE		0x0
#define	ET_1ST			0x1
#define	ET_2ND			0x2
#define	ET_3RD			0x3

/////FORCE //////
#define PWM_HIGH        0x2
#define PWM_LOW         0x1
//--------------------------------
// HRPWM (High Resolution PWM)
//================================
// HRCNFG
#define	HR_Disable		0x0
#define	HR_REP			0x1
#define	HR_FEP			0x2
#define	HR_BEP			0x3

#define	HR_CMP			0x0
#define	HR_PHS			0x1

#define	HR_CTR_ZERO		0x0
#define	HR_CTR_PRD		0x1

// HRpwm frequency
#define RESOLUTION_10BIT   23
#define RESOLUTION_12BIT   93
#define RESOLUTION_14BIT   374

#define P_IO_EPWM1_A 0
#define P_IO_EPWM1_B 1

#define P_IO_EPWM3_A 4
#define P_IO_EPWM3_B 5

#define P_IO_EPWM4_A 6
#define P_IO_EPWM4_B 7

#define P_IO_EPWM5_A 8
#define P_IO_EPWM5_B 9

#define P_IO_EPWM6_A 10
#define P_IO_EPWM6_B 11

// AQCTLA and AQCTLB (Action Qualifier Control)
//=============================================
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define	AQ_NO_ACTION	0x0
#define	AQ_CLEAR		0x1
#define	AQ_SET			0x2
#define	AQ_TOGGLE		0x3

/////FORCE //////
#define PWM_FORCE_LOW        0x2
#define PWM_FORCE_HIGH        0x1
#define PWM_FORCE_NOACTIVE    0x0
extern void fnPWM_Init(void);
extern void fnPWM_RealTime(void);

extern void fnPWM_EPWM1_Init(void);

extern void fnPWM_SetFreq(volatile struct EPWM_REGS *epwmRegs, U16 freq, U16 *rsp);


extern void fnPWM_SetaDuty(volatile struct EPWM_REGS *epwmRegs, U16 duty);
extern void fnPWM_SetaDutyPercent(volatile struct EPWM_REGS *epwmRegs, U16 freq, U16 dutypercent);

extern void fnPWM_SetbDuty(volatile struct EPWM_REGS *epwmRegs,U16 duty);

extern void fnPWM_ForceA(volatile struct EPWM_REGS *epwmRegs, U8 csfa);
extern void fnPWM_ForceB(volatile struct EPWM_REGS *epwmRegs, U8 csfb);
extern void fnPWM_highsideActive(volatile struct EPWM_REGS *epwmRegs);
extern void fnPWM_EPWM4_Init(void);
extern void fnPWM_EPWM5_Init(void);
extern void fnPWM_EPWM6_Init(void);
extern void fnPWM_EPWM3_Init(void);




#endif
