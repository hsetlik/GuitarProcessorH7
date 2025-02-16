/*
 * DelayLine.cpp
 *
 *  Created on: Feb 7, 2025
 *      Author: hayden
 */
#include "DelayLine.h"

uint16_t neededBufSize(uint16_t delay) {
	uint16_t sigBits = 0;
	while (delay) {
		sigBits++;
		delay >>= 1;
	}
	return (1 << sigBits);
}

DelayLine::DelayLine() {

}

void DelayLine::init(uint16_t size) {
	maxDelay = size;
	length = neededBufSize(maxDelay);
	// allocate the buffer
	data = new float[length];
	// initialize the bitmask
	mask = length - 1;
	// set the main delay tap to max
	offsets[TAP_MAIN] = mask + 1 - maxDelay;
}

void DelayLine::setDelay(uint8_t tap, uint16_t delay) {
	offsets[tap] = mask + 1 - delay;
}

DelayLine::~DelayLine() {
	delete[] data;
}

float DelayLine::process(uint16_t cycle, float input) {
	write(cycle, input);
	return read(TAP_MAIN, cycle);
}

void DelayLine::write(uint16_t cycle, float input) {
	data[cycle & mask] = input;
}

float DelayLine::read(uint8_t tap, uint16_t cycle) {
	return data[(cycle + offsets[tap]) & mask];
}

