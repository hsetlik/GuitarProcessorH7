/*
 * BandShifter.h
 *
 *  Created on: Feb 18, 2025
 *      Author: hayden
 */

#ifndef INC_OCTAVESHIFTER_H_
#define INC_OCTAVESHIFTER_H_

#ifdef __cplusplus
#include "main.h"
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>
#include "FastSqrt.h"
/*
 * Polyphonic octave up/down effect based on the Steve Schulteis's implementation
 * of Etienne Thuillier's paper from 2016: https://core.ac.uk/download/pdf/80719011.pdf
 * original code here: https://github.com/schult/terrarium-poly-octave
 * */

// how many band shifters we should use
#define NUM_SHIFTERS 80
//=============================================================================
class BandShifter {
public:
	BandShifter() = default;

	BandShifter(float center, float sample_rate, float bw);
	void update(float sample);

	float up1() const {
		return _up1;
	}

	float down1() {
		return _down1.real();
	}

	float down2() const {
		return _down2;
	}

private:

	float _d0 = 0;
	std::complex<float> _d1;
	std::complex<float> _d2;
	std::complex<float> _c1;
	std::complex<float> _c2;

	std::complex<float> _s1;
	std::complex<float> _s2;

	std::complex<float> _y;
	float _up1 = 0;
	std::complex<float> _down1;
	float _down2 = 0;

	float _down1_sign = 1;
	float _down2_sign = 1;

	void update_filter(float sample);
	void update_up1();

	void update_down1();

	void update_down2();
};

//=============================================================================

class OctaveShifter{
private:
	std::vector<BandShifter> shifters;
public:
	// control the level of each octave
	float upLevel = 0.85f;
	float downLevel = 0.85f;
	float dryLevel = 0.85f;
	OctaveShifter();
	float process(float input);

};
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_OCTAVESHIFTER_H_ */
