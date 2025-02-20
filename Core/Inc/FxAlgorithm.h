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
#include <memory>
#include "Dattorro.h"
#include "FFTPhaser.h"
// Yes im using inheritance here i promise it makes sense
// this is a base class from which all effects will derive
class FxAlgorithm {
public:
	FxAlgorithm();
	virtual ~FxAlgorithm();
	// main callback to be overridden, handles a single sample
	virtual void processChunk(float* input, float* output, uint32_t blockSize)=0;
	// callback for a parameter value changing
	virtual void paramChanged(uint8_t id, uint16_t value)=0;
};

// handy smart pointer alias
typedef std::unique_ptr<FxAlgorithm> alg_ptr_t;

//Child Classes for each effect===============================================
class FFTPhaserAlgo : public FxAlgorithm {
private:
	FFTPhaser phaser;
public:
	FFTPhaserAlgo();
	void processChunk(float* in, float* out, uint32_t size) override;
	void paramChanged(uint8_t id, uint16_t value) override;
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
