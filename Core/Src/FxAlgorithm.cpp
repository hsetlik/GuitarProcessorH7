/*
 * FxAlgorithm.cpp
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#include "FxAlgorithm.h"

FxAlgorithm::FxAlgorithm(){

}

FxAlgorithm::~FxAlgorithm(){

}
//==============================================================================
FFTPhaserAlgo::FFTPhaserAlgo() {

}

void FFTPhaserAlgo::processChunk(float* in, float* out, uint32_t size){
	for(uint32_t i = 0; i < size; ++i){
		out[i] = phaser.process(in[i]);
	}
}


void FFTPhaserAlgo::paramChanged(uint8_t id, uint16_t value){
	switch(id){
	case 0: // Knob 1: selects the phasing type
		phaser.setType_u16(value);
		break;
	case 1: // knob 2: Sets the phasing depth
		phaser.setDepth_u16(value);
		break;
	case 2: // knob 3: sets the LFO speed
		phaser.setSpeed_u16(value);
		break;
	default:
		break;
	}
}


