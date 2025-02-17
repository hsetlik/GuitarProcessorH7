/*
 * PeakDetector.cpp
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */
#include "EnvelopeDetector.h"

EnvelopeDetector::EnvelopeDetector(){
	filter.setup(AUDIO_SAMPLE_RATE, 200.0f);
}

float EnvelopeDetector::process(float input){
	static float prevInput = 0.0f;
	// zero crossing found
	if((prevInput < 0.0f) != (input  < 0.0f)){
		lastPeakMagnitude = currentPeakMagnitude;
		currentPeakMagnitude = 0.0f;
	}
	float inputAbs = std::fabs(input);
	if(inputAbs > currentPeakMagnitude){
		currentPeakMagnitude = inputAbs;
	}

	prevInput = input;
	return filter.filter(lastPeakMagnitude);
}
