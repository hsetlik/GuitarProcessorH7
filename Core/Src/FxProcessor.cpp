/*
 * FxProcessor.cpp
 *
 *  Created on: Jan 27, 2025
 *      Author: hayden
 */
#include "FxProcessor.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

pedal_state_t getDefaultPedalState(){
	pedal_state_t s;
	s.alg = 0;
	s.fxOn = 1;
	s.knobA = KNOB_MID;
	s.knobB = KNOB_MID;
	s.knobC = KNOB_MID;
	s.exp = 0;
	return s;
}
//============================

FxProcessor::FxProcessor() : state(getDefaultPedalState()){
	//TODO: initialize each algo here

}

void FxProcessor::processChunk(uint16_t numSamples, float* input, float* output){
#ifdef CODEC_TEST
	for(uint16_t i = 0; i < numSamples; ++i){
		meter.tick(input[i]);
		output[i] = input[i];
	}

#else
	algs[state.alg]->processChunk(input, output, (uint32_t)numSamples);
#endif

}

void FxProcessor::controlMoved(uint8_t id, uint16_t value){
	switch(id){
	case 0:
		state.knobA = value;
		break;
	case 1:
		state.knobB = value;
		break;
	case 2:
		state.knobC = value;
		break;
	case 3:
		state.exp = value;
		break;
	default:
		break;
	}
	//algs[state.alg]->paramChanged(id, value);
}


void FxProcessor::advanceAlg(){
	state.alg = (state.alg + 1) % 6;
}


uint8_t FxProcessor::getLEDByte(){
	/* In LSB-first order the LEDs are wired like:
	 * 0: Alg 5
	 * 1: Alg 4
	 * 2: Alg 3
	 * 3: Alg 2
	 * 4: Alg 1
	 * 5: Alg 0
	 * 6: Tempo
	 * 7: Bypass
	 * */
	uint8_t byte = 0x00;
	// switch the appropriate alg. LED on
	uint8_t mask = 0x01 << (5 - state.alg);
	byte = byte | mask;
	if(state.fxOn){
		byte = byte | (0x01 << 7);
	}
	return byte;
}

void FxProcessor::updateDisplay(){
	// 1. clear the display
	ssd1306_Fill(Black);
	// 2. draw the RMS label text
	ssd1306_SetCursor(1, 0);
	const char* rmsTitle = "RMS Level: ";
	ssd1306_WriteString((char*)rmsTitle, Font_7x10, White);

	// 3. draw the numerical RMS level
	auto rmsStr = std::to_string(meter.getRMSLevel());
	ssd1306_SetCursor(1, 11);
	ssd1306_WriteString((char*)rmsStr.c_str(), Font_16x24, White);

}


void FxProcessor::setBypass(bool effectOn){
	state.fxOn = effectOn ? 1 : 0;
}


//================================================================================

fx_processor_t create_fx_processor(){
	return new FxProcessor();
}

void process_fx(fx_processor_t proc, uint16_t size, float* input, float* output){
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->processChunk(size, input, output);
}

void fx_control_moved(fx_processor_t proc, uint8_t id, uint16_t value){
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->controlMoved(id, value);
}

uint8_t fx_get_led_byte(fx_processor_t proc){
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	return ptr->getLEDByte();
}


void fx_advance_alg(fx_processor_t proc){
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->advanceAlg();
}

void fx_set_bypass(fx_processor_t proc, uint8_t bypass){

	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->setBypass(bypass > 0);
}


void fx_update_display(fx_processor_t proc) {
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->updateDisplay();
}

