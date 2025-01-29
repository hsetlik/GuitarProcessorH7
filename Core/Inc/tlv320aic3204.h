/*
 * tlv320aic3204.h
 *
 *  Created on: Jan 29, 2025
 *      Author: hayden
 */

#ifndef INC_TLV320AIC3204_H_
#define INC_TLV320AIC3204_H_
#include "main.h"

// define the relevant pins and handles

#define TLV_DEVICE_ADDR 0x30
#define TLV_I2C hi2c2
#define TLV_I2S hi2s1
#define TLV_NRST_GPIO_Port CODEC_NRST_GPIO_Port
#define TLV_NRST_Pin CODEC_NRST_Pin


#ifdef __cplusplus
extern "C" {
#endif

// helpers for setting/reading registers
HAL_StatusTypeDef TLV_writeRegister(uint8_t page, uint8_t addr, uint8_t data);
HAL_StatusTypeDef TLV_readRegister(uint8_t page, uint8_t addr, uint8_t* data);






#ifdef __cplusplus
}
#endif

#endif /* INC_TLV320AIC3204_H_ */
