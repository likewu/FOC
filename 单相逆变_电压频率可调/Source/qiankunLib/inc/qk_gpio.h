/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_gpio.h
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-1
Description		--

**************************************************************************/
#ifndef __QK_GPIO_H
#define __QK_GPIO_H

#include "power.h"

// LED
#define LED0_ON()	(GPIOF->BSRR =  GPIO_Pin_0)
#define LED1_ON()	(GPIOF->BSRR =  GPIO_Pin_1)
#define LED2_ON()	(GPIOF->BSRR =  GPIO_Pin_2)
#define LED3_ON()	(GPIOF->BSRR =  GPIO_Pin_3)
#define LED4_ON()	(GPIOF->BSRR =  GPIO_Pin_4)
#define LED5_ON()	(GPIOF->BSRR =  GPIO_Pin_5)
#define LED6_ON()	(GPIOF->BSRR =  GPIO_Pin_6)
#define LED7_ON()	(GPIOF->BSRR =  GPIO_Pin_7)
#define LED0_OFF()	(GPIOF->BRR =  GPIO_Pin_0)
#define LED1_OFF()	(GPIOF->BRR =  GPIO_Pin_1)
#define LED2_OFF()	(GPIOF->BRR =  GPIO_Pin_2)
#define LED3_OFF()	(GPIOF->BRR =  GPIO_Pin_3)
#define LED4_OFF()	(GPIOF->BRR =  GPIO_Pin_4)
#define LED5_OFF()	(GPIOF->BRR =  GPIO_Pin_5)
#define LED6_OFF()	(GPIOF->BRR =  GPIO_Pin_6)
#define LED7_OFF()	(GPIOF->BRR =  GPIO_Pin_7)
#define LED_ALL_ON()	(GPIOF->BSRR =  0xff)
#define LED_ALL_OFF()	(GPIOF->BRR =  0xff)

// KEY
#define	KEY0		((GPIOC->IDR & GPIO_Pin_13)==0)
#define	KEY1		((GPIOE->IDR & GPIO_Pin_5)==0)
#define	KEY2		((GPIOE->IDR & GPIO_Pin_3)==0)
#define	KEY3		((GPIOE->IDR & GPIO_Pin_1)==0)
#define	KEY4		((GPIOE->IDR & GPIO_Pin_6)==0)
#define	KEY5		((GPIOE->IDR & GPIO_Pin_4)==0)
#define	KEY6		((GPIOE->IDR & GPIO_Pin_2)==0)
#define	KEY7		((GPIOE->IDR & GPIO_Pin_0)==0)
#define	KEY_ON		(1)
#define	KEY_OFF		(0)

void GpioInit(void);

#endif 
