/*
 * FFTPhaser.h
 *
 *  Created on: Feb 19, 2025
 *      Author: hayden
 */

#ifndef INC_FFTPHASER_H_
#define INC_FFTPHASER_H_
#include "main.h"

#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif

#ifndef _ARM_MATH_H
#include "arm_math.h"
#endif
#ifdef __cplusplus
#include "DSPMath.h"
// this will be a tradeoff between bandwidth and latency I think
#define FFT_SIZE 256
#define REV_INDEX_TABLE_LENGTH 440


class FFTPhaser {
private:
	// FFT and associated things
	arm_cfft_instance_f32 fft;
	// these arrays get initialized in the constructor
	float twiddleCoef[FFT_SIZE * 2];
	uint16_t revIndexTable[REV_INDEX_TABLE_LENGTH];



	float aBuf[FFT_SIZE * 2];
	float bBuf[FFT_SIZE * 2];
	uint16_t bufferIdx = 0;
	float* processPtr;
	float* playbackPtr;
public:
	FFTPhaser();

};



#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_FFTPHASER_H_ */
