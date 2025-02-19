/*
 * SineLFO.h
 *
 *  Created on: Feb 19, 2025
 *      Author: hayden
 */

#ifndef INC_SINELFO_H_
#define INC_SINELFO_H_
#include "main.h"
#ifdef __cplusplus
#include "DSPMath.h"
#include <cmath>

class SineLFO{
private:
	float phase = 0.0f;
public:
	SineLFO()=default;
	float update(float hz){
		float delta = hz / AUDIO_SAMPLE_RATE;
		phase = std::fmod(phase + delta, 1.0f);
		return std::sin(phase * DSP::twoPi);

	}
};



#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_SINELFO_H_ */
