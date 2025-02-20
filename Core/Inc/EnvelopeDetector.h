/*
 * PeakDetector.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_ENVELOPEDETECTOR_H_
#define INC_ENVELOPEDETECTOR_H_


#ifdef __cplusplus

#include "Butterworth.h"
#include "FastSqrt.h"

// a simple moving average filter class
class MovingAverage{
private:
	uint16_t size;
	uint16_t head;
	float* data;
public:
	MovingAverage(uint16_t size=5);
	~MovingAverage();
	float process(float input);

};

class EnvelopeDetector{
private:
	bool inPositivePhase = true;
	float lastPeakMagnitude = 0.0f;
	float currentPeakMagnitude = 0.0f;
	DSP::Butterworth::LowPass<4> filter;


public:
	EnvelopeDetector();
	// process the input sample and return the current envelope level
	float process(float input);


};

//=================================================
#define RMS_WINDOW_MAX 4000
#define RMS_WINDOW_MIN 800
// this is equivalent to about 30 values per second
#define RMS_WINDOW_DEFAULT 1600
class RMSDetector {
	float output = 0.0f;
	float squareSum = 0.0f;
	uint16_t sampleIdx = 0;
	uint16_t windowSize = RMS_WINDOW_DEFAULT;
public:
	RMSDetector()=default;
	// handle the input sample and return the value of the last computed window
	float process(float input);
};

#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_ENVELOPEDETECTOR_H_ */
