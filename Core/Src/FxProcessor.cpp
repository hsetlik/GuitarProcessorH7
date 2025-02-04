/*
 * FxProcessor.cpp
 *
 *  Created on: Jan 27, 2025
 *      Author: hayden
 */
#include "FxProcessor.h"

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

}

void FxProcessor::processChunk(uint16_t numSamples, float* input, float* output){

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
	//TODO: pass updated parameters to the fx algo here
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




