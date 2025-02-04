/*
 * tlv320aic3204.c
 *
 *  Created on: Jan 29, 2025
 *      Author: hayden
 */
#include "tlv320aic3204.h"

// little helper for the page scheme
void TLV_selectPage(uint8_t page) {
	static uint8_t currentPage = 0xFF;
	// check if we need to select the page
	if (page != currentPage) {
		HAL_StatusTypeDef pgStatus = HAL_I2C_Mem_Write(&TLV_I2C,
				TLV_DEVICE_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &page, 1,
				HAL_MAX_DELAY);
		if (pgStatus != HAL_OK) {
			Error_Handler();
		}
		currentPage = page;
	}
}

HAL_StatusTypeDef TLV_writeRegister(uint8_t page, uint8_t addr, uint8_t data) {
	TLV_selectPage(page);
	return HAL_I2C_Mem_Write(&TLV_I2C, TLV_DEVICE_ADDR, addr,
			I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef TLV_readRegister(uint8_t page, uint8_t addr, uint8_t *data) {
	TLV_selectPage(page);
	return HAL_I2C_Mem_Read(&TLV_I2C, TLV_DEVICE_ADDR, addr,
			I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

uint8_t TLV_verifyRegister(uint8_t page, uint8_t addr, uint8_t expected) {
	uint8_t memory = 0x00;
	if (TLV_readRegister(page, addr, &memory) != HAL_OK) {
		Error_Handler();
	}
	if (expected != memory) {
		return 0;
	}
	return 1;
}

HAL_StatusTypeDef TLV_initCodec(tlv_register_t *settings, uint16_t size) {
	// step 1: cycle the NRST pin
	HAL_GPIO_WritePin(TLV_NRST_GPIO_Port, TLV_NRST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(TLV_NRST_GPIO_Port, TLV_NRST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	// step 2: perform hardware reset
	HAL_StatusTypeDef resetStatus = TLV_writeRegister(TLV_softwareReset_pg,
			TLV_softwareReset_reg, 0x01);
	if (resetStatus != HAL_OK) {
		return resetStatus;
	}

	// step 3: set up each register value passed in
	for (uint16_t i = 0; i < size; i++) {
		HAL_StatusTypeDef regStatus = TLV_writeRegister(settings[i].page,
				settings[i].address, settings[i].value);
		if (regStatus != HAL_OK) {
			return regStatus;
		}
	}
#ifdef TLV_VERIFY_SETUP
	// step 4: verify that the correct value was written to each register
	for (uint16_t i = 0; i < size; i++) {
		if(TLV_verifyRegister(settings[i].page, settings[i].address, settings[i].value) < 1){
			return HAL_ERROR;
		}
	}
#endif
	return HAL_OK;
}

//======================================================================


HAL_StatusTypeDef TLV_quickInit_monoGuitarPedal(){
	uint16_t idx = 0;
	tlv_register_t settings[50];

	// use the internal LDOs
	settings[idx] = (tlv_register_t){TLV_ldoControl_pg, TLV_ldoControl_reg, 0x01};
	idx++;



	// run the init
	return TLV_initCodec(settings, idx);



}
