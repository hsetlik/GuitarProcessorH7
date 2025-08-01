/*
 * FxProcessor.h
 *
 *  Created on: Jan 27, 2025
 *      Author: hayden
 */

#ifndef INC_FXPROCESSOR_H_
#define INC_FXPROCESSOR_H_
#include "FxAlgorithm.h"
#include "RollingRMS.h"
#ifdef __cplusplus

#define CODEC_TEST

class FxProcessor{
private:
	pedal_state_t state;
	alg_ptr_t algs[6];
	Dattorro reverb;
	RollingRMS meter;

public:
	FxProcessor();
	void processChunk(uint16_t numSamples, float* input, float* output);
	void controlMoved(uint8_t id, uint16_t value);
	void advanceAlg();
	void setBypass(bool effectOn);
	// this updates the OLED contents
	void updateDisplay();
	// this returns a bitfield of which LEDs should be lit up
	uint8_t getLEDByte();

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
EXTERNC void fx_control_moved(fx_processor_t proc, uint8_t id, uint16_t value);
EXTERNC uint8_t fx_get_led_byte(fx_processor_t proc);
EXTERNC void fx_advance_alg(fx_processor_t proc);
EXTERNC void fx_set_bypass(fx_processor_t proc, uint8_t bypass);
EXTERNC void fx_update_display(fx_processor_t proc);

#undef EXTERNC

#endif /* INC_FXPROCESSOR_H_ */
