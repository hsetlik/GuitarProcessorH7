/*
 * Compressor.h
 *
 *  Created on: Feb 20, 2025
 *      Author: hayden
 */

#ifndef INC_COMPRESSOR_H_
#define INC_COMPRESSOR_H_

#ifdef __cplusplus
#include "EnvelopeDetector.h"
#include "DSPMath.h"

class Compressor {
private:
	EnvelopeDetector ed;
};

#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_COMPRESSOR_H_ */
