/*
 * PeakDetector.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_PEAKDETECTOR_H_
#define INC_PEAKDETECTOR_H_
#include "main.h"

#define PEAK_BUFFER_SIZE 64

#ifdef __cplusplus
#include "RingBuffer.h"
struct PeakData {
	float value;
	uint32_t duration;
};

class PeakDetector{
private:
	float currentPeakValue = 0.0f;
	uint32_t currentPhaseDuration = 0;
	uint8_t negativePhase = 0;
	RingBuffer<PEAK_BUFFER_SIZE, PeakData> peakBuffer;

public:
	PeakDetector(){
	}
	void push(float input);


};

#endif

#ifdef __cplusplus
#define EXTERNC extern "c"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_PEAKDETECTOR_H_ */
