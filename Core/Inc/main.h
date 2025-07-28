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
// represents the state of the pedal's controls at a given moment
typedef struct {
	uint8_t fxOn;
	uint8_t alg;
	uint16_t knobA;
	uint16_t knobB;
	uint16_t knobC;
	uint16_t exp;
}pedal_state_t;

#define KNOB_MAX 65535
#define KNOB_MID 32767

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BUFFER_SIZE 256
#define BUFFER_FLOAT_SIZE 64
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
// conversion between float and uint32 for the codec
//float u32_to_float(uint32_t val);
//uint32_t float_to_u32(float val);
float i32_to_float(int32_t val);
int32_t float_to_i32(float val);
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
#define CODEC_NRST_Pin GPIO_PIN_15
#define CODEC_NRST_GPIO_Port GPIOE
#define CODEC_SCL_Pin GPIO_PIN_10
#define CODEC_SCL_GPIO_Port GPIOB
#define CODEC_SDA_Pin GPIO_PIN_11
#define CODEC_SDA_GPIO_Port GPIOB
#define LED_LATCH_Pin GPIO_PIN_12
#define LED_LATCH_GPIO_Port GPIOB
#define LED_SCK_Pin GPIO_PIN_13
#define LED_SCK_GPIO_Port GPIOB
#define LED_NRST_Pin GPIO_PIN_14
#define LED_NRST_GPIO_Port GPIOB
#define LED_DATA_Pin GPIO_PIN_15
#define LED_DATA_GPIO_Port GPIOB
#define BYP_Pin GPIO_PIN_9
#define BYP_GPIO_Port GPIOA
#define MODE_Pin GPIO_PIN_10
#define MODE_GPIO_Port GPIOA
#define TAP_Pin GPIO_PIN_11
#define TAP_GPIO_Port GPIOA
#define DISP_SCL_Pin GPIO_PIN_6
#define DISP_SCL_GPIO_Port GPIOB
#define DISP_SDA_Pin GPIO_PIN_7
#define DISP_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern I2C_HandleTypeDef hi2c2;
extern I2S_HandleTypeDef hi2s1;
//extern RNG_HandleTypeDef hrng;

// handy access to the actual sample rate pre current clock settings
#define AUDIO_SAMPLE_RATE 48828.0f

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
