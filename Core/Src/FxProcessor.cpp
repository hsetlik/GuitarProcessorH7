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




