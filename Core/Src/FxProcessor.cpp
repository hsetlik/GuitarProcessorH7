/*
 * FxProcessor.cpp
 *
 *  Created on: Jan 27, 2025
 *      Author: hayden
 */
#include "FxProcessor.h"

FxProcessor::FxProcessor(){

}

void FxProcessor::processChunk(uint16_t numSamples, float* input, float* output){

}


//================================================================================

fx_processor_t create_fx_processor(){
	return new FxProcessor();
}

void process_audio_chunk(fx_processor_t proc, uint16_t size, float* input, float* output){
	FxProcessor* ptr = static_cast<FxProcessor*>(proc);
	ptr->processChunk(size, input, output);
}




