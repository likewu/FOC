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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

#define ADC_CH1 ADC_CHANNEL_12
#define ADC_CH2 ADC_CHANNEL_13
#define ADC_CH3 ADC_CHANNEL_14
#define ADC_CH4 ADC_CHANNEL_15


#define ADC_CHANNEL_REF 5
#define ADC_CHANNEL_TEMP 4
extern uint16_t adc1ValBuf[ADC_CHANNEL_REF + 1];
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
extern float AdcGetChipTemperature();
extern float AdcGetVoltage(uint32_t _channel);
extern uint16_t AdcGetRaw(uint32_t _channel);

int64_t GetEncoderCount(TIM_TypeDef *tim);
int64_t GetCntLoop(TIM_TypeDef *tim);
void ClearCntLoop(TIM_TypeDef *tim);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define KEY_Pin GPIO_PIN_13
#define KEY_GPIO_Port GPIOC
#define OLED_I2C2_SCL_Pin GPIO_PIN_10
#define OLED_I2C2_SCL_GPIO_Port GPIOB
#define OLED_I2C2_SDA_Pin GPIO_PIN_11
#define OLED_I2C2_SDA_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOC
#define IMU_I2C1_SCL_Pin GPIO_PIN_6
#define IMU_I2C1_SCL_GPIO_Port GPIOB
#define IMU_I2C1_SDA_Pin GPIO_PIN_7
#define IMU_I2C1_SDA_GPIO_Port GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
