/*
 * DelayLine.h
 *
 *  Created on: Feb 7, 2025
 *      Author: hayden
 */

#ifndef INC_DELAYLINE_H_
#define INC_DELAYLINE_H_
#include "main.h"

#ifdef __cplusplus
class DelayLine{
private:
	const uint16_t length;
	uint16_t idx = 0;
	float* data;
public:
	DelayLine(uint16_t size);
	~DelayLine();
	float process(float input);
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
