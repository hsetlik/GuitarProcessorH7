/*
 * tlv320aic3204.c
 *
 *  Created on: Jan 29, 2025
 *      Author: hayden
 */
#include "tlv320aic3204.h"
#include <stdint.h>
#include "main.h"
#include "stm32h7xx_hal_def.h"

//static uint16_t TLV_DEVICE_ADDR = (0x18 << 1);
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
		Error_Handler();
		return 0;
	}
	return 1;
}


HAL_StatusTypeDef TLV_initCodec(tlv_register_t *settings, uint16_t size) {
	// step 1: cycle the NRST pin
	HAL_GPIO_WritePin(TLV_NRST_GPIO_Port, TLV_NRST_Pin, GPIO_PIN_RESET);
	HAL_Delay(25);
	HAL_GPIO_WritePin(TLV_NRST_GPIO_Port, TLV_NRST_Pin, GPIO_PIN_SET);
	HAL_Delay(25);
	// step 2: perform hardware reset
	HAL_StatusTypeDef resetStatus = TLV_writeRegister(TLV_softwareReset_pg,
			TLV_softwareReset_reg, 0x01);
	if (resetStatus != HAL_OK) {
		return resetStatus;
	}
	HAL_Delay(10);

	// step 3: set up each register value passed in
	for (uint16_t i = 0; i < size; i++) {
		HAL_StatusTypeDef regStatus = TLV_writeRegister(settings[i].page,
				settings[i].address, settings[i].value);
		if (regStatus != HAL_OK) {
			Error_Handler();
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

bool TLV_leftDcMeasurementReady(){
	uint8_t data;
	HAL_StatusTypeDef readStatus = TLV_readRegister(TLV_intFlag3_pg, TLV_intFlag3_reg, &data);
	if(readStatus != HAL_OK){
		Error_Handler();
	}
	const uint8_t mask = 0b00000100;
	return (mask & data) > 0;
}


bool TLV_rightDcMeasurementReady(){
	uint8_t data;
	HAL_StatusTypeDef readStatus = TLV_readRegister(TLV_intFlag3_pg, TLV_intFlag3_reg, &data);
	if(readStatus != HAL_OK){
		Error_Handler();
	}
	const uint8_t mask = 0b00000010;
	return (mask & data) > 0;
}


HAL_StatusTypeDef TLV_enableDcMeasurement(){
	uint8_t data = 0b11000001;
	return TLV_writeRegister(TLV_dcMeasurement1_pg, TLV_dcMeasurement1_reg, data);
}


uint32_t TLV_getDcMeasurementLeft(){
	// latch the data into the output register
	HAL_StatusTypeDef latchOnStatus = TLV_writeRegister(TLV_dcMeasurement2_pg, TLV_dcMeasurement2_reg, 0b01000000);
	if(latchOnStatus != HAL_OK){
		Error_Handler();
	}
	// get bits 23-16
	uint8_t bits1 = 0;
	HAL_StatusTypeDef status1 = TLV_readRegister(TLV_leftDcOutput1_pg, TLV_leftDcOutput1_reg, &bits1);
	if(status1 != HAL_OK){
		Error_Handler();
	}
	// get bits 15-8
	uint8_t bits2 = 0;
	HAL_StatusTypeDef status2 = TLV_readRegister(TLV_leftDcOutput2_pg, TLV_leftDcOutput2_reg, &bits2);
	if(status2 != HAL_OK){
		Error_Handler();
	}
	// get bits 7-0
	uint8_t bits3 = 0;
	HAL_StatusTypeDef status3 = TLV_readRegister(TLV_leftDcOutput3_pg, TLV_leftDcOutput3_reg, &bits3);
	if(status3 != HAL_OK){
		Error_Handler();
	}
	uint32_t output = 0;
	output |= (uint32_t)(bits1 << 16);
	output |= (uint32_t)(bits2 << 8);
	output |= (uint32_t)(bits3);
	// un-latch the read register
	HAL_StatusTypeDef latchOffStatus = TLV_writeRegister(TLV_dcMeasurement2_pg, TLV_dcMeasurement2_reg, 0b00000000);
	if(latchOffStatus != HAL_OK){
		Error_Handler();
	}
	return output;
}

uint32_t TLV_getDcMeasurementRight(){
	// get bits 23-16
	uint8_t bits1 = 0;
	HAL_StatusTypeDef status1 = TLV_readRegister(TLV_rightDcOutput1_pg, TLV_rightDcOutput1_reg, &bits1);
	if(status1 != HAL_OK){
		Error_Handler();
	}
	// get bits 15-8
	uint8_t bits2 = 0;
	HAL_StatusTypeDef status2 = TLV_readRegister(TLV_rightDcOutput2_pg, TLV_rightDcOutput2_reg, &bits2);
	if(status2 != HAL_OK){
		Error_Handler();
	}
	// get bits 7-0
	uint8_t bits3 = 0;
	HAL_StatusTypeDef status3 = TLV_readRegister(TLV_rightDcOutput3_pg, TLV_rightDcOutput3_reg, &bits3);
	if(status3 != HAL_OK){
		Error_Handler();
	}
	uint32_t output = 0;
	output |= (uint32_t)(bits1 << 16);
	output |= (uint32_t)(bits2 << 8);
	output |= (uint32_t)(bits3);
	return output;
}


//======================================================================


HAL_StatusTypeDef TLV_quickInit_monoGuitarPedal(){
	uint16_t idx = 0;
	tlv_register_t settings[100];
	// setup based on TI's application note SLA557: https://www.ti.com/lit/ml/slaa557/slaa557.pdf?ts=1752009023928
	// and the register map from p. 35-39 of the datasheet: https://www.ti.com/lit/ds/symlink/tlv320aic3204.pdf?ts=1752054663881&ref_url=https%253A%252F%252Fwww.mouser.fr%252F

	// ADC setup stuff-----------------------------------------
	// set NADC divider to 1
	settings[idx] = (tlv_register_t){TLV_NADC_pg, TLV_NADC_reg, 0b10000001};
	++idx;
	// set MADC divider to 2
	settings[idx] = (tlv_register_t){TLV_MADC_pg, TLV_MADC_reg, 0b10000010};
	++idx;
	// set audio word length
	settings[idx] = (tlv_register_t){TLV_audioIntSetting1_pg, TLV_audioIntSetting1_reg, TLV_wordLength_32};
	++idx;
	// bit clock offset
//	settings[idx] = (tlv_register_t){TLV_audioBitOffset_pg, TLV_audioBitOffset_reg, 5};
//	++idx;
	// set ADC oversampling to 128
	settings[idx] = (tlv_register_t){TLV_adcOversampling_pg, TLV_adcOversampling_reg, 0x80};
	++idx;

	// use the internal LDOs
	settings[idx] = (tlv_register_t){TLV_ldoControl_pg, TLV_ldoControl_reg, 0x01};
	++idx;
	// set up common mode voltages
	settings[idx] = (tlv_register_t){TLV_commonModeControl_pg, TLV_commonModeControl_reg, TLV_commonMode_075};
	++idx;
	// set the analog quick charge time to 3.1ms
	settings[idx] = (tlv_register_t){TLV_analogQuickCharge_pg, TLV_analogQuickCharge_reg, 0b00110001};
	++idx;
	// set the reference charging time to 40ms
	settings[idx] = (tlv_register_t){TLV_refBootTime_pg, TLV_refBootTime_reg, 0x01};
	++idx;


	// our differential input circuit will use the right MICPGA, with IN1_R routed to the
	// positive input and IN1_L to the negative

	// IN1_R gets routed to the pos. input with 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGAPosRouting_pg, TLV_rightMICPGAPosRouting_reg, 0x80};
	++idx;
	// IN1_L gets routed to the neg. input also via 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGANegRouting_pg, TLV_rightMICPGANegRouting_reg, 0b01010000};
	++idx;
	// unmute right MICPGA and set unity gain
	settings[idx] = (tlv_register_t){TLV_rightMICPGAGain_pg, TLV_rightMICPGAGain_reg, 0x0C};
	++idx;
	// power on right ADC channel
	settings[idx] = (tlv_register_t){TLV_adcChannelSetup_pg, TLV_adcChannelSetup_reg, 0b01000000};
	++idx;
	// unmute the right ADC vis fine gain adjust
	settings[idx] = (tlv_register_t){TLV_adcFineGainAdjust_pg, TLV_adcFineGainAdjust_reg, 0b10000000};
	++idx;

	// DAC setup stuff------------------------------------------------------
	// Per the application note, the the master input clock (12.288 Mhz) is equal to NDAC * MDAC * DOSR
	// set NDAC divider to 2
	settings[idx] = (tlv_register_t){TLV_NDAC_pg, TLV_NDAC_reg, 0b10000010};
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
	//settings[idx] = (tlv_register_t){TLV_dacSignalProcessingBlock_pg, TLV_dacSignalProcessingBlock_reg, 0x05};
	//++idx;
	// route right (negative) DAC to left line out
	settings[idx] = (tlv_register_t){TLV_lolRouting_pg, TLV_lolRouting_reg, 0b00010000};
	++idx;
	// route right DAC to right line out
	settings[idx] = (tlv_register_t){TLV_lorRouting_pg, TLV_lorRouting_reg, 0b00001000};
	++idx;
	// unmute LOL driver and set gain to -6db
	settings[idx] = (tlv_register_t){TLV_lolDriverGain_pg, TLV_lolDriverGain_reg, 0x3A};
	++idx;
	// unmute LOR driver and set gain to -6db
	settings[idx] = (tlv_register_t){TLV_lorDriverGain_pg, TLV_lorDriverGain_reg, 0x3A};
	++idx;
	// power up the left and right line out drivers
	settings[idx] = (tlv_register_t){TLV_outputDriverPower_pg, TLV_outputDriverPower_reg, 0b00001100};
	++idx;

	// route & power up the right DAC
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup1_pg, TLV_dacChannelSetup1_reg, 0b01000100};
	++idx;
	// unmute the DAC
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup2_pg, TLV_dacChannelSetup2_reg, 0b00001000};
	++idx;
	// run the init
	return TLV_initCodec(settings, idx);

}

// Stereo setup------------------------------------------------------------------------------------------
HAL_StatusTypeDef TLV_quickInit_stereoGuitarPedal(){
	uint16_t idx = 0;
	tlv_register_t settings[100];
	// setup based on TI's application note SLA557: https://www.ti.com/lit/ml/slaa557/slaa557.pdf?ts=1752009023928
	// and the register map from p. 35-39 of the datasheet: https://www.ti.com/lit/ds/symlink/tlv320aic3204.pdf?ts=1752054663881&ref_url=https%253A%252F%252Fwww.mouser.fr%252F

	// ADC setup stuff-----------------------------------------
	// set NADC divider to 1
	settings[idx] = (tlv_register_t){TLV_NADC_pg, TLV_NADC_reg, 0b10000001};
	++idx;
	// set MADC divider to 2
	settings[idx] = (tlv_register_t){TLV_MADC_pg, TLV_MADC_reg, 0b10000010};
	++idx;
	// set audio word length
	settings[idx] = (tlv_register_t){TLV_audioIntSetting1_pg, TLV_audioIntSetting1_reg, TLV_wordLength_32};
	++idx;

	// set ADC oversampling to 128
	settings[idx] = (tlv_register_t){TLV_adcOversampling_pg, TLV_adcOversampling_reg, 0x80};
	++idx;

	// use the internal LDOs
	settings[idx] = (tlv_register_t){TLV_ldoControl_pg, TLV_ldoControl_reg, 0x01};
	++idx;
	// set up common mode voltages
	settings[idx] = (tlv_register_t){TLV_commonModeControl_pg, TLV_commonModeControl_reg, TLV_commonMode_075};
	++idx;
	// set the analog quick charge time to 3.1ms
	settings[idx] = (tlv_register_t){TLV_analogQuickCharge_pg, TLV_analogQuickCharge_reg, 0b00110001};
	++idx;
	// set the reference charging time to 40ms
	settings[idx] = (tlv_register_t){TLV_refBootTime_pg, TLV_refBootTime_reg, 0x01};
	++idx;

	// Right channel input setup-
	// differential input to right MICPGA, with IN1_R routed to the
	// positive input and IN1_L to the negative
	// IN1_R gets routed to the pos. input with 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGAPosRouting_pg, TLV_rightMICPGAPosRouting_reg, 0b10000000};
	++idx;
	// IN1_L gets routed to the neg. input also via 20k input impedance
	settings[idx] = (tlv_register_t){TLV_rightMICPGANegRouting_pg, TLV_rightMICPGANegRouting_reg, 0b01010000};
	++idx;
	// unmute right MICPGA and set unity gain
	settings[idx] = (tlv_register_t){TLV_rightMICPGAGain_pg, TLV_rightMICPGAGain_reg, 0x0C};
	++idx;

	// Left channel input, similar setup to above-
	// IN2_L goes to positive terminal @ 20k	
	settings[idx] = (tlv_register_t){TLV_leftMICPGAPosRouting_pg, TLV_leftMICPGAPosRouting_reg, 0b00100000};
	++idx;
	// IN2_R goes to negative terminal @ 20k
	settings[idx] = (tlv_register_t){TLV_leftMICPGANegRouting_pg, TLV_leftMICPGANegRouting_reg, 0b00100000};
	++idx;
	// unmute left MICPGA and set unity gain
	settings[idx] = (tlv_register_t){TLV_leftMICPGAVolumeControl_pg, TLV_leftMICPGAVolumeControl_reg, 0x0C};
	++idx;
	// power on both ADC channels
	settings[idx] = (tlv_register_t){TLV_adcChannelSetup_pg, TLV_adcChannelSetup_reg, 0b11000000};
	++idx;
	// unmute both ADCs via fine gain adjust
	settings[idx] = (tlv_register_t){TLV_adcFineGainAdjust_pg, TLV_adcFineGainAdjust_reg, 0b00000000};
	++idx;

	// DAC setup stuff------------------------------------------------------
	// Per the application note, the the master input clock (12.288 Mhz) is equal to NDAC * MDAC * DOSR
	// set NDAC divider to 2
	settings[idx] = (tlv_register_t){TLV_NDAC_pg, TLV_NDAC_reg, 0b10000010};
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
	// settings[idx] = (tlv_register_t){TLV_dacSignalProcessingBlock_pg, TLV_dacSignalProcessingBlock_reg, 0x05};
	// ++idx;
	// route left DAC to left line out
	settings[idx] = (tlv_register_t){TLV_lolRouting_pg, TLV_lolRouting_reg, 0b00001000};
	++idx;
	// route right DAC to right line out
	settings[idx] = (tlv_register_t){TLV_lorRouting_pg, TLV_lorRouting_reg, 0b00001000};
	++idx;
	// unmute LOL driver and set gain to -6db
	settings[idx] = (tlv_register_t){TLV_lolDriverGain_pg, TLV_lolDriverGain_reg, 0x3A};
	++idx;
	// unmute LOR driver and set gain to -6db
	settings[idx] = (tlv_register_t){TLV_lorDriverGain_pg, TLV_lorDriverGain_reg, 0x3A};
	++idx;
	// power up the left and right line out drivers
	settings[idx] = (tlv_register_t){TLV_outputDriverPower_pg, TLV_outputDriverPower_reg, 0b00001100};
	++idx;

	// route & power up the DACs
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup1_pg, TLV_dacChannelSetup1_reg, 0b11010100};
	++idx;
	// unmute the DACs
	settings[idx] = (tlv_register_t){TLV_dacChannelSetup2_pg, TLV_dacChannelSetup2_reg, 0b00000000};
	++idx;
	// run the init
	return TLV_initCodec(settings, idx);

}



// troubleshooting stuff-------------------------------------------
void TLV_checkFlags(){
	uint8_t flagReg1, flagReg2;
	TLV_readRegister(TLV_dacFlag1_pg, TLV_dacFlag1_reg,  &flagReg1);
	TLV_readRegister(TLV_dacFlag2_pg, TLV_dacFlag2_reg,  &flagReg2);
	uint8_t expectedFlags1 = 0b01001100;
	if(flagReg1 != expectedFlags1){
		Error_Handler();
	}
	uint8_t expectedFlags2 = 0b00010001;
	if(flagReg2 != expectedFlags2){
		Error_Handler();
	}

}
