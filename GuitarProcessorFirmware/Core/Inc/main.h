/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define AUDIO_BUF_SIZE 256
#define AUDIO_SAMPLE_RATE 46875.0f

#define isample_t int32_t

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */


// stuff for driving the LEDs
bool ledUpdateDue();
void updateLedData(uint8_t data);
// drive the debug points
void setDBG1Level(bool isHigh);
void setDBG2Level(bool isHigh);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_C_Pin GPIO_PIN_0
#define ADC_C_GPIO_Port GPIOC
#define ADC_B_Pin GPIO_PIN_1
#define ADC_B_GPIO_Port GPIOC
#define ADC_D_Pin GPIO_PIN_0
#define ADC_D_GPIO_Port GPIOA
#define ADC_A_Pin GPIO_PIN_2
#define ADC_A_GPIO_Port GPIOA
#define LED_LATCH_Pin GPIO_PIN_4
#define LED_LATCH_GPIO_Port GPIOA
#define LED_CLK_Pin GPIO_PIN_5
#define LED_CLK_GPIO_Port GPIOA
#define LED_DATA_Pin GPIO_PIN_7
#define LED_DATA_GPIO_Port GPIOA
#define LED_NRST_Pin GPIO_PIN_4
#define LED_NRST_GPIO_Port GPIOC
#define SW_1_Pin GPIO_PIN_2
#define SW_1_GPIO_Port GPIOB
#define SW_2_Pin GPIO_PIN_7
#define SW_2_GPIO_Port GPIOE
#define CODEC_NRST_Pin GPIO_PIN_15
#define CODEC_NRST_GPIO_Port GPIOE
#define CODEC_SCL_Pin GPIO_PIN_10
#define CODEC_SCL_GPIO_Port GPIOB
#define CODEC_SDA_Pin GPIO_PIN_11
#define CODEC_SDA_GPIO_Port GPIOB
#define MODE_Pin GPIO_PIN_8
#define MODE_GPIO_Port GPIOC
#define BYP_Pin GPIO_PIN_9
#define BYP_GPIO_Port GPIOC
#define DBG1_Pin GPIO_PIN_1
#define DBG1_GPIO_Port GPIOD
#define DBG2_Pin GPIO_PIN_2
#define DBG2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
extern I2C_HandleTypeDef hi2c2;
extern I2S_HandleTypeDef hi2s2;
extern SPI_HandleTypeDef hspi1;
#define LED_SPI hspi1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
