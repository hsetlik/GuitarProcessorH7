/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BUFFER_SIZE 128

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_C_Pin GPIO_PIN_0
#define ADC_C_GPIO_Port GPIOC
#define ADC_B_Pin GPIO_PIN_1
#define ADC_B_GPIO_Port GPIOC
#define ADC_A_Pin GPIO_PIN_2
#define ADC_A_GPIO_Port GPIOA
#define ADC_EXP_Pin GPIO_PIN_3
#define ADC_EXP_GPIO_Port GPIOA
#define ENC_L_Pin GPIO_PIN_12
#define ENC_L_GPIO_Port GPIOB
#define ENC_R_Pin GPIO_PIN_13
#define ENC_R_GPIO_Port GPIOB
#define ENC_B_Pin GPIO_PIN_14
#define ENC_B_GPIO_Port GPIOB
#define TEMPO_LED_Pin GPIO_PIN_14
#define TEMPO_LED_GPIO_Port GPIOD
#define BYP_LED_Pin GPIO_PIN_15
#define BYP_LED_GPIO_Port GPIOD
#define PIXELS_Pin GPIO_PIN_6
#define PIXELS_GPIO_Port GPIOC
#define BTN_R_Pin GPIO_PIN_9
#define BTN_R_GPIO_Port GPIOC
#define BTN_L_Pin GPIO_PIN_8
#define BTN_L_GPIO_Port GPIOA
#define BYP_Pin GPIO_PIN_9
#define BYP_GPIO_Port GPIOA
#define TAP_Pin GPIO_PIN_10
#define TAP_GPIO_Port GPIOA
#define MODE_Pin GPIO_PIN_11
#define MODE_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
