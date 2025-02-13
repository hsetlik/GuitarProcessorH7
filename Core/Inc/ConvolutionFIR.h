/*
 * Filter.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_CONVOLUTIONFIR_H_
#define INC_CONVOLUTIONFIR_H_

#include "main.h"

#ifndef ARM_MATH_CM7
#define ARM_MATH_CM7
#endif

#ifndef _ARM_MATH_H
#include "arm_math.h"
#endif
#define CD_KERNEL_GAIN 1.0f
#define CD_KERNEL_LENGTH 173
#define CD_BLOCK_LENGTH (BUFFER_SIZE / 4)

extern const float CD_Kernel[CD_KERNEL_LENGTH];

#ifdef __cplusplus


class ConvDirectFIR {
private:
	arm_fir_instance_f32 firProc;
	// intermediate buf for the FIR processing
	float firBuf[CD_KERNEL_LENGTH + CD_BLOCK_LENGTH - 1];
	float kernelTimeReversed[CD_KERNEL_LENGTH];
public:
	ConvDirectFIR(){

	}
	void init(const float* kernel);
	void processChunk(float* input, float* output, uint16_t length);
};

#endif

#ifdef __cplusplus
#define EXTERNC extern "c"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_CONVOLUTIONFIR_H_ */
