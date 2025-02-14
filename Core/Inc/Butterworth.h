/*
 * Butterworth.h
 *
 *  Created on: Feb 14, 2025
 *      Author: hayden
 */

#ifndef INC_BUTTERWORTH_H_
#define INC_BUTTERWORTH_H_
#include "IIR.h"

#ifdef __cplusplus
namespace DSP{
namespace Butterworth{

// Analog low pass prototype (s-plane)
class AnalogLowPass : public LayoutBase {
public:
	AnalogLowPass();
	void design(const int poles);
private:
	int numPoles = 0;
};

// Analog low shelf prototype (s-plane)
class AnalogLowShelf : public LayoutBase {
public:
	AnalogLowShelf();
	void design(const int poles, float gain);
private:
	int numPoles = 0;
	float gainDb = 0.0f;
};


} // namespace Butterworth
} // namespace DSP



#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_BUTTERWORTH_H_ */
