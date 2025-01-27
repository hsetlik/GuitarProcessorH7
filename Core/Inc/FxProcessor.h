/*
 * FxProcessor.h
 *
 *  Created on: Jan 27, 2025
 *      Author: hayden
 */

#ifndef INC_FXPROCESSOR_H_
#define INC_FXPROCESSOR_H_
#include "main.h"

#ifdef __cplusplus

class FxProcessor{
public:
	FxProcessor();
	void processChunk(uint16_t numSamples, float* input, float* output);
	void controlMoved(uint8_t id, uint16_t value);

};

#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

typedef void* fx_processor_t;

EXTERNC fx_processor_t create_fx_processor();
// main processing callback
EXTERNC void process_fx(fx_processor_t proc, uint16_t size, float* input, float* output);


#undef EXTERNC

#endif /* INC_FXPROCESSOR_H_ */
