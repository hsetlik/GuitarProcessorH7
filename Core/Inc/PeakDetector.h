/*
 * PeakDetector.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_PEAKDETECTOR_H_
#define INC_PEAKDETECTOR_H_
#include "main.h"


#ifdef __cplusplus
#include "RingBuffer.h"

class PeakDetector{
private:
	float prevPeakValue = 0.0f;
	uint32_t timeSincePrevPeak = 0;

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
