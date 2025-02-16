/*
 * DelayLine.h
 *
 *  Created on: Feb 7, 2025
 *      Author: hayden
 */

#ifndef INC_DELAYLINE_H_
#define INC_DELAYLINE_H_
#include "main.h"

enum {
  TAP_MAIN = 0,
  TAP_OUT1,
  TAP_OUT2,
  TAP_OUT3,
  MAX_TAPS
};
#ifdef __cplusplus
class DelayLine{
private:
	uint16_t maxDelay;
	uint16_t length;
	float* data = nullptr;
	uint16_t mask = 0;
public:
	uint16_t offsets[MAX_TAPS];
	DelayLine();
	~DelayLine();
	void init(uint16_t delay);
	// set the length of a delay tap
	void setDelay(uint8_t tap, uint16_t delaySamples);
	// read/write access
	float process(uint16_t cycle, float input);
	void write(uint16_t cycle, float input);
	float read(uint8_t tap, uint16_t cycle);
};


#endif

#ifdef __cplusplus
#define EXTERNC extern "c"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_DELAYLINE_H_ */
