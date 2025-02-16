/*
 * Dattorro.h
 *
 *  Created on: Feb 16, 2025
 *      Author: hayden
 */

#ifndef INC_DATTORRO_H_
#define INC_DATTORRO_H_
/* Dattorro Reverb implementation ======================================
 * Based on the reverb described in his 1997 paper found here:
 * https://ccrma.stanford.edu/~dattorro/EffectDesignPart1.pdf
 * largely a C++-ified version of the 'dattorro-verb' C implementation:
 * https://github.com/el-visio/dattorro-verb
 * */

#ifdef __cplusplus
#include "DelayLine.h"
#define MAX_PREDELAY 4400


class Dattorro {
private:
	// in order of the main algorithm:
	DelayLine preDelay;
	float preFilter;
	// input diffusors
	DelayLine inDiffusion[4];

	// the two 'tanks'
	DelayLine decayDiffusion1[2];
	DelayLine decayDiffusion2[2];
	DelayLine preDampingDelay[2];
	DelayLine postDampingDelay[2];
	float damping[2];
	// cycle to pass the delay lines
	uint16_t t;
public:
	// reverb settings
	float preFilterAmt;
	float inputDiff1Amt;
	float inputDiff2Amt;
	float decayDiff1Amt;
	float decayDiff2Amt;
	float dampingAmt;
	float decayAmt;
	//=====================================================
	Dattorro();
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

#endif /* INC_DATTORRO_H_ */
