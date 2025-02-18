/*
 * OctaveShifter.cpp
 *
 *  Created on: Feb 18, 2025
 *      Author: hayden
 */

#include "OctaveShifter.h"
#include <cmath>
#include <complex>
#include <numbers>

BandShifter::BandShifter(float center, float sample_rate, float bw) {
	constexpr double pi = 3.1415926535897932384626433832795028841971;
	constexpr auto j = std::complex<double>(0, 1);

	const auto w0 = pi * bw / sample_rate;
	const auto cos_w0 = std::cos(w0);
	const auto sin_w0 = std::sin(w0);
	const auto sqrt_2 = std::sqrt(2.0);
	const auto a0 = (1 + sqrt_2 * sin_w0 / 2);
	const auto g = (1 - cos_w0) / (2 * a0);

	const auto w1 = 2 * pi * center / sample_rate;
	const auto e1 = std::exp(j * w1);
	const auto e2 = std::exp(j * w1 * 2.0);

	const auto d0 = g;
	const auto d1 = e1 * 2.0 * g;
	const auto d2 = e2 * g;
	const auto c1 = e1 * (-2 * cos_w0) / a0;
	const auto c2 = e2 * (1 - sqrt_2 * sin_w0 / 2) / a0;

	_d0 = d0;
	_d1 = std::complex<float>(d1.real(), d1.imag());
	_d2 = std::complex<float>(d2.real(), d2.imag());
	_c1 = std::complex<float>(c1.real(), c1.imag());
	_c2 = std::complex<float>(c2.real(), c2.imag());
}

void BandShifter::update(float sample) {
	update_filter(sample);
	update_up1();
	update_down1();
	//update_down2();
}

// Prototype filter is LPF from "Cookbook formulae for audio EQ biquad
// filter coefficients", a.k.a. "Audio EQ Cookbook",
// by Robert Bristow-Johnson
// https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
//
// Transformed as described in Section 3.1 of "Complex Band-Pass Filters
// for Analytic Signal Generation and Their Application" by Andrew J. Noga
// https://apps.dtic.mil/sti/tr/pdf/ADA395963.pdf

void BandShifter::update_filter(float sample) {
	const auto prev_y = _y;
	_y = _s2 + _d0 * sample;
	_s2 = _s1 + _d1 * sample - _c1 * _y;
	_s1 = _d2 * sample - _c2 * _y;

	if ((_y.real() < 0)
			&& (std::signbit(_y.imag()) != std::signbit(prev_y.imag()))) {
		_down1_sign = -_down1_sign;
	}
}

// Octave shifts are performed via phase scaling described in "Real-Time
// Polyphonic Octave Doubling for the Guitar" by Etienne Thuillier
// https://core.ac.uk/download/pdf/80719011.pdf
//
// in = complex input signal
// out = scaled complex output signal
// g = scaling factor
//
// out = in * (in / |in|)^(g - 1)
//
// Note that for octave down (g = 1/2), it is necessary to detect phase
// transitions in order to set the sign of the output signal.

void BandShifter::update_up1() {
	const auto a = _y.real();
	const auto b = _y.imag();
	_up1 = (a * a - b * b) * fastInvSqrt(a * a + b * b);
}

void BandShifter::update_down1() {
	const auto a = _y.real();
	const auto b = _y.imag();
	const auto b_sign = (b < 0) ? -1.0f : 1.0f;

	const auto x = 0.5f * a * fastInvSqrt(a * a + b * b);
	const auto c = fastSqrt(0.5f + x);
	const auto d = b_sign * fastSqrt(0.5f - x);

	const auto prev_down1 = _down1;
	_down1 = _down1_sign
			* std::complex<float>((a * c + b * d), (b * c - a * d));

	if ((_down1.real() < 0)
			&& (std::signbit(_down1.imag()) != std::signbit(prev_down1.imag()))) {
		_down2_sign = -_down2_sign;
	}
}

void BandShifter::update_down2() {
	const auto a = _down1.real();
	const auto b = _down1.imag();
	const auto b_sign = (b < 0) ? -1.0f : 1.0f;

	const auto x = 0.5f * a * fastInvSqrt(a * a + b * b);
	const auto c = fastSqrt(0.5f + x);
	const auto d = b_sign * fastSqrt(0.5f - x);

	_down2 = _down2_sign * (a * c + b * d);
}

//=============================================================================

constexpr float centerFreq(int16_t band){
	return 480.0f * std::pow<float>(2.0f, 0.027f * (float)band) - 420.0f;
}

constexpr float getBandwidth(int16_t band){
	const float f0 = centerFreq(band - 1);
	const float f1 = centerFreq(band);
	const float f2 = centerFreq(band + 1);
	const float a = f2 - f1;
	const float b = f1 - f0;
	return 2.0f * (a * b) / (a + b);
}

OctaveShifter::OctaveShifter() {
	for (int16_t i = 0; i < NUM_SHIFTERS; i++) {
		float cf = centerFreq(i);
		float bandwidth = getBandwidth(i);
		shifters.emplace_back(cf, AUDIO_SAMPLE_RATE, bandwidth);
	}
}

float OctaveShifter::process(float input){
	float up = 0.0f;
	float down = 0.0f;
	for(auto& shifter : shifters){
		shifter.update(input);
		up += shifter.up1();
		down += shifter.down1();
	}
	return (input * dryLevel) + (up * upLevel) + (down * downLevel);
}

