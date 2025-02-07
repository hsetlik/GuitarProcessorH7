/*
 * PeakDetector.cpp
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */
#include "PeakDetector.h"



void PeakDetector::push(float input){
	// 1. check if we're at a zero crossing
	if((negativePhase && input >= 0.0f) || (!negativePhase && input < 0.0f)){

		peakBuffer.push((PeakData){currentPeakValue, currentPhaseDuration});
		currentPeakValue = 0.0f;
		currentPhaseDuration = 0;
		negativePhase = (input >= 0.0f) ? 0 : 1;
	}
	float inMagnitude = (negativePhase) ? std::fabs(input) : input;
	if(inMagnitude >= currentPeakValue){
		currentPeakValue = inMagnitude;
	}
	++currentPhaseDuration;
}


