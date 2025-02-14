/*
 * IIR.h
 *
 *  Created on: Feb 13, 2025
 *      Author: hayden
 */

#ifndef INC_IIR_H_
#define INC_IIR_H_
#include "main.h"

#define DEFAULT_IIR_ORDER 4



#ifdef __cplusplus

#include "DSPMath.h"

// Biquad IIR class based on the IIR1 library: https://github.com/berndporr/iir1



class IIRFilter {
private:
	const uint16_t length;
	float* aCoeffs;
	float* bCoeffs;
	float* inBuf;
	float* outBuf;
public:
	IIRFilter(uint16_t size, float* aBuf, float* bBuf);
	~IIRFilter();
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

#endif /* INC_IIR_H_ */
