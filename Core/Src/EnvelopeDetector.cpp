/*
 * PeakDetector.cpp
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */
#include "EnvelopeDetector.h"

MovingAverage::MovingAverage(uint16_t s) : size(s), head(0){
	data = new float[size];
}

MovingAverage::~MovingAverage(){
	delete [] data;
}


float MovingAverage::process(float input){
	// add the input to the ring buffer
	data[head] = input;
	head = (head + 1) % size;
	// add the samples
	float sum = 0.0f;
	for(uint16_t i = 0; i < size; i++){
		sum += data[i];
	}
	// return the average
	return sum / (float)size;
}


//=================================================

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
