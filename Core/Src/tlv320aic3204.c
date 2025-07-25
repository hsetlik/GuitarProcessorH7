/*
 * tlv320aic3204.c
 *
 *  Created on: Jan 29, 2025
 *      Author: hayden
 */
#include "tlv320aic3204.h"

// little helper for the page scheme
static HAL_StatusTypeDef TLV_selectPage(uint8_t page) {
	static uint8_t currentPage = 0xFF;
	// check if we need to select the page
	if (page != currentPage) {
#ifdef I2C_USE_MEM_WRITE
		HAL_StatusTypeDef pgStatus = HAL_I2C_Mem_Write(&TLV_I2C, TLV_DEVICE_ADDR, 0x00, 1, &page, 1, HAL_MAX_DELAY);
#else
		uint8_t bytes[2] = {0x00, page};
		HAL_StatusTypeDef pgStatus = HAL_I2C_Master_Transmit(&TLV_I2C, TLV_DEVICE_ADDR, bytes, 2, HAL_MAX_DELAY);
#endif
		if (pgStatus != HAL_OK) {
			return pgStatus;
		}
		currentPage = page;
	}
	return HAL_OK;
}

 static HAL_StatusTypeDef TLV_writeRegister(uint8_t page, uint8_t addr, uint8_t data) {
	HAL_StatusTypeDef pgStatus = TLV_selectPage(page);
	if(pgStatus != HAL_OK)
		return pgStatus;
#ifdef I2C_USE_MEM_WRITE
	return HAL_I2C_Mem_Write(&TLV_I2C, TLV_DEVICE_ADDR, addr, 1, &data, 1, HAL_MAX_DELAY);
#else
	uint8_t bytes[2] = {addr, data};
	return HAL_I2C_Master_Transmit(&TLV_I2C, TLV_DEVICE_ADDR, bytes, 2, HAL_MAX_DELAY);
#endif
}

static HAL_StatusTypeDef TLV_readRegister(uint8_t page, uint8_t addr, uint8_t *data) {
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
	HAL_Delay(2);
	HAL_GPIO_WritePin(TLV_NRST_GPIO_Port, TLV_NRST_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
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
	for (uint16_t i = 1; i < size; i++) {
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
	tlv_register_t settings[100];
	// setup based on TI's application note SLA557: https://www.ti.com/lit/ml/slaa557/slaa557.pdf?ts=1752009023928
	// and the register map from p. 35-39 of the datasheet: https://www.ti.com/lit/ds/symlink/tlv320aic3204.pdf?ts=1752054663881&ref_url=https%253A%252F%252Fwww.mouser.fr%252F

	// software reset in register 1:
	settings[idx] = (tlv_register_t){TLV_softwareReset_pg, TLV_softwareReset_reg, 0b00000001};
	++idx;


	// ADC setup stuff-----------------------------------------
	// set NADC divider to 1
	settings[idx] = (tlv_register_t){TLV_NADC_pg, TLV_NADC_reg, 0b10000001};
	++idx;
	// set MADC divider to 2
	settings[idx] = (tlv_register_t){TLV_MADC_pg, TLV_MADC_reg, 0b10000010};
	++idx;
	// set 32 bit word length
	settings[idx] = (tlv_register_t){TLV_audioIntSetting1_pg, TLV_audioIntSetting1_reg, 0b00110000};
	++idx;
	// bit clock offset
//	settings[idx] = (tlv_register_t){TLV_audioBitOffset_pg, TLV_audioBitOffset_reg, 5};
//	++idx;
	// set ADC oversampling to 128
	settings[idx] = (tlv_register_t){TLV_adcOversampling_pg, TLV_adcOversampling_reg, 0x80};
	++idx;

	// use the internal LDOs
	settings[idx] = (tlv_register_t){TLV_ldoControl_pg, TLV_ldoControl_reg, 0x01};
	idx++;
	// set the analog quick charge time to 3.1ms
	settings[idx] = (tlv_register_t){TLV_analogQuickCharge_pg, TLV_analogQuickCharge_reg, 0b00110001};
	idx++;
	// set the reference charging time to 40ms
	settings[idx] = (tlv_register_t){TLV_refBootTime_pg, TLV_refBootTime_reg, 0x01};
	idx++;


	// our differential input circuit will use the right MICPGA, with IN1_R routed to the
	// positive input and IN1_L to the negative

	// IN1_R gets routed to the pos. input with 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGAPosRouting_pg, TLV_rightMICPGAPosRouting_reg, 0x80};
	idx++;
	// IN1_L gets routed to the neg. input also via 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGANegRouting_pg, TLV_rightMICPGANegRouting_reg, 0b01010000};
	idx++;
	// unmute right MICPGA and set unity gain
	settings[idx] = (tlv_register_t){TLV_rightMICPGAGain_pg, TLV_rightMICPGAGain_reg, 0x0C};
	idx++;
	// power on right ADC channel
	settings[idx] = (tlv_register_t){TLV_adcChannelSetup_pg, TLV_adcChannelSetup_reg, 0b01000000};
	idx++;
	// unmute the right ADC vis fine gain adjust
	settings[idx] = (tlv_register_t){TLV_adcFineGainAdjust_pg, TLV_adcFineGainAdjust_reg, 0b10000000};
	idx++;

	// DAC setup stuff------------------------------------------------------
	// set NDAC divider to 1
	settings[idx] = (tlv_register_t){TLV_NDAC_pg, TLV_NDAC_reg, 0b10000001};
	++idx;
	// set MDAC divider to 2
	settings[idx] = (tlv_register_t){TLV_MDAC_pg, TLV_MDAC_reg, 0b10000010};
	++idx;
	// set the DAC OSR to 128
	settings[idx] = (tlv_register_t){TLV_DACOSR1_pg, TLV_DACOSR1_reg, 0x00};
	++idx;
	settings[idx] = (tlv_register_t){TLV_DACOSR2_pg, TLV_DACOSR2_reg, 0x80};
	++idx;
	// select PRB_R5
	settings[idx] = (tlv_register_t){TLV_dacSignalProcessingBlock_pg, TLV_dacSignalProcessingBlock_reg, 0x05};
	++idx;
	// route left DAC to left line out
	settings[idx] = (tlv_register_t){TLV_lolRouting_pg, TLV_lolRouting_reg, 0b00001000};
	++idx;
	// unmute LOL driver and set gain to 0db
	settings[idx] = (tlv_register_t){TLV_lolDriverGain_pg, TLV_lolDriverGain_reg, 0x00};
	++idx;
	// power up the left line out driver
	settings[idx] = (tlv_register_t){TLV_outputDriverPower_pg, TLV_outputDriverPower_reg, 0b00001000};
	++idx;
	// route & power up the left DAC
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup1_pg, TLV_dacChannelSetup1_reg, 0b10100000};
	++idx;
	// unmute the DAC
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup2_pg, TLV_dacChannelSetup2_reg, 0b00000100};
	++idx;
	// run the init
	return TLV_initCodec(settings, idx);



}
