/*
 * IIR.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: hayden
 */

#include "IIR.h"

IIRFilter::IIRFilter(uint16_t size, float* aBuf, float* bBuf): length(size){
	// put our 3 buffers on the heap
	aCoeffs = new float[length];
	bCoeffs = new float[length];
	inBuf = new float[length];
	outBuf = new float[length];
	// copy the coeffs in and zero the feedback buffer
	for(uint16_t i = 0; i < length; ++i){
		aCoeffs[i] = aBuf[i];
		bCoeffs[i] = bBuf[i];
		inBuf[i] = 0.0f;
		outBuf[i] = 0.0f;
	}
}

IIRFilter::~IIRFilter(){
	delete[] aCoeffs;
	delete[] bCoeffs;
	delete[] inBuf;
	delete[] outBuf;
}

float IIRFilter::process(float input){
	static uint16_t pos = 0;
	float output = 0.0f;
	// add the input into the buffer
	inBuf[pos] = input;
	pos = (pos + 1) % length;
	// calculate the input side
	for(uint16_t i = 0; i < length; ++i){
		output += bCoeffs[i] * inBuf[pos];
	}
	// calculate the output side
	for(uint16_t i = 1; i < length; ++i){
		// subtract 1 from the index bc pos was already incremented above
		output -= aCoeffs[i] * outBuf[(pos + i - 1) % length];
	}
	// add back to the feedback buf
	outBuf[pos] = output;
	return output;
}

