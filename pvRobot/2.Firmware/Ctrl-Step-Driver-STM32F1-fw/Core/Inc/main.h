/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void CAN_Send(CAN_TxHeaderTypeDef* pHeader, uint8_t* data);

extern void (*OnRecvEnd)(uint8_t *data, uint16_t len);
void Uart_SetRxCpltCallBack(void(*xerc)(uint8_t *, uint16_t));

extern void MX_TIM1_Init(void);
extern void MX_TIM2_Init(void);
extern void MX_TIM3_Init(void);
extern void MX_TIM4_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define POWER_U_Pin GPIO_PIN_0
#define POWER_U_GPIO_Port GPIOA
#define DRV_TEMP_Pin GPIO_PIN_1
#define DRV_TEMP_GPIO_Port GPIOA
#define HW_ELEC_BM_Pin GPIO_PIN_2
#define HW_ELEC_BM_GPIO_Port GPIOA
#define HW_ELEC_BP_Pin GPIO_PIN_3
#define HW_ELEC_BP_GPIO_Port GPIOA
#define HW_ELEC_AM_Pin GPIO_PIN_4
#define HW_ELEC_AM_GPIO_Port GPIOA
#define HW_ELEC_AP_Pin GPIO_PIN_5
#define HW_ELEC_AP_GPIO_Port GPIOA
#define SIGNAL_COUNT_DIR_Pin GPIO_PIN_7
#define SIGNAL_COUNT_DIR_GPIO_Port GPIOA
#define SIGNAL_COUNT_DIR_EXTI_IRQn EXTI9_5_IRQn
#define SIGNAL_COUNT_EN_Pin GPIO_PIN_0
#define SIGNAL_COUNT_EN_GPIO_Port GPIOB
#define SIGNAL_ALERT_Pin GPIO_PIN_1
#define SIGNAL_ALERT_GPIO_Port GPIOB
#define BUTTON2_Pin GPIO_PIN_2
#define BUTTON2_GPIO_Port GPIOB
#define HW_ELEC_BPWM_Pin GPIO_PIN_10
#define HW_ELEC_BPWM_GPIO_Port GPIOB
#define HW_ELEC_APWM_Pin GPIO_PIN_11
#define HW_ELEC_APWM_GPIO_Port GPIOB
#define BUTTON1_Pin GPIO_PIN_12
#define BUTTON1_GPIO_Port GPIOB
#define SPI1_CS_Pin GPIO_PIN_15
#define SPI1_CS_GPIO_Port GPIOA

extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t TxData[8];
extern uint8_t RxData[8];
extern uint32_t TxMailbox;

extern uint16_t whole_adc_data[2][12];

#define BUFFER_SIZE  128

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern volatile uint8_t rxLen;
extern volatile uint8_t recv_end_flag;
extern uint8_t rx_buffer[BUFFER_SIZE];

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
