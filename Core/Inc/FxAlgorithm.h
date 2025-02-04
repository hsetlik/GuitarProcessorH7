/*
 * FxAlgorithm.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_FXALGORITHM_H_
#define INC_FXALGORITHM_H_
#include "main.h"

#ifdef __cplusplus

// Yes im using inheritance here i promise it makes sense
// this is a base class from which all effects will derive
class FxAlgorithm {
public:
	FxAlgorithm();
	virtual ~FxAlgorithm();
	// main callback to be overridden, handles a single sample
	virtual float process(float input)=0;
	//
};

#endif

#ifdef __cplusplus
#define EXTERNC extern "c"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_FXALGORITHM_H_ */
