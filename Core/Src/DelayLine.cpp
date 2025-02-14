/*
 * DelayLine.cpp
 *
 *  Created on: Feb 7, 2025
 *      Author: hayden
 */
#include "DelayLine.h"

DelayLine::DelayLine(uint16_t size) : length(size){
	data = new float[length];
}

DelayLine::~DelayLine(){
	delete [] data;
}

float DelayLine::process(float input){
	data[idx] = input;
	idx = (idx + 1) % length;
	return data[idx];
}



