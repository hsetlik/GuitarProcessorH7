/*
 * IIR.cpp
 *
 *  Created on: Feb 13, 2025
 *      Author: hayden
 */

#include "IIR.h"
namespace DSP {

BiquadPoleState::BiquadPoleState(const Biquad &s) {
	const float a0 = s.getA0();
	const float a1 = s.getA1();
	const float a2 = s.getA2();
	const float b0 = s.getB0();
	const float b1 = s.getB1();
	const float b2 = s.getB2();

	if (a2 == 0 && b2 == 0) {
		// single pole
		poles.first = -a1;
		zeros.first = -b0 / b1;
		poles.second = 0;
		zeros.second = 0;
	} else {
		{
			const complex_t c = sqrt(complex_t(a1 * a1 - 4 * a0 * a2, 0));
			float d = 2. * a0;
			poles.first = -(a1 + c) / d;
			poles.second = (c - a1) / d;
			if (poles.is_nan())
				Error_Handler();
		}

		{
			const complex_t c = sqrt(complex_t(b1 * b1 - 4 * b0 * b2, 0));
			float d = 2. * b0;
			zeros.first = -(b1 + c) / d;
			zeros.second = (c - b1) / d;
			if (zeros.is_nan())
				Error_Handler();
		}
	}

	gain = b0 / a0;
}
//----------------------------
float Biquad::filter_DirectFormI(float input) {
	const float output = b[0] * input + (b[1] * x[0]) + (b[2] * x[1])
			- (a[1] * y[0]) - (a[2] * y[1]);
	x[1] = x[0];
	y[1] = y[0];
	x[0] = input;
	y[0] = output;
	return output;
}

complex_t Biquad::response(float normFrequency) const {
	const float a0 = getA0();
	const float a1 = getA1();
	const float a2 = getA2();
	const float b0 = getB0();
	const float b1 = getB1();
	const float b2 = getB2();

	const float w = 2 * doublePi * normFrequency;
	const complex_t czn1 = std::polar<float>(1.0f, -w);
	const complex_t czn2 = std::polar<float>(1., -2.0f * w);
	complex_t ch(1);
	complex_t cbot(1);

	complex_t ct(b0 / a0);
	complex_t cb(1);
	ct = addmul(ct, b1 / a0, czn1);
	ct = addmul(ct, b2 / a0, czn2);
	cb = addmul(cb, a1 / a0, czn1);
	cb = addmul(cb, a2 / a0, czn2);
	ch *= ct;
	cbot *= cb;

	return ch / cbot;
}

std::vector<PoleZeroPair> Biquad::getPoleZeros() const {
	std::vector < PoleZeroPair > vec = { };
	BiquadPoleState bps(*this);
	vec.push_back(bps);
	return vec;
}

void Biquad::setCoeffs(float a0, float a1, float a2, float b0, float b1,
		float b2) {
	if (!std::isnan(a0)) {
		a[0] = a0;
	} else {
		Error_Handler();
	}

	if (!std::isnan(a1)) {
		a[1] = a1 / a0;
	} else {
		Error_Handler();
	}

	if (!std::isnan(a2)) {
		a[2] = a2 / a0;
	} else {
		Error_Handler();
	}

	if (!std::isnan(b0)) {
		b[0] = b0 / a0;
	} else {
		Error_Handler();
	}

	if (!std::isnan(b1)) {
		b[1] = b1 / a0;
	} else {
		Error_Handler();
	}

	if (!std::isnan(b2)) {
		b[2] = b2 / a0;
	} else {
		Error_Handler();
	}
}

void Biquad::setOnePole(complex_t pole, complex_t zero) {
	if (pole.imag() != 0)
		Error_Handler();
	if (zero.imag() != 0)
		Error_Handler();

	const float a0 = 1;
	const float a1 = -pole.real();
	const float a2 = 0;
	const float b0 = 1;
	const float b1 = -zero.real();
	const float b2 = 0;

	setCoeffs(a0, a1, a2, b0, b1, b2);
}

void Biquad::setTwoPole(complex_t pole1, complex_t zero1, complex_t pole2,
		complex_t zero2) {
	const float a0 = 1;
	float a1;
	float a2;

	if (pole1.imag() != 0) {
		if (pole2 != std::conj(pole1))
			Error_Handler();
		a1 = -2 * pole1.real();
		a2 = std::norm(pole1);
	} else {
		if (pole2.imag() != 0)
			Error_Handler();
		a1 = -(pole1.real() + pole2.real());
		a2 = pole1.real() * pole2.real();
	}

	const float b0 = 1;
	float b1;
	float b2;

	if (zero1.imag() != 0) {
		if (zero2 != std::conj(zero1))
			Error_Handler();
		b1 = -2 * zero1.real();
		b2 = std::norm(zero1);
	} else {
		if (zero2.imag() != 0)
			Error_Handler();

		b1 = -(zero1.real() + zero2.real());
		b2 = zero1.real() * zero2.real();
	}

	setCoeffs(a0, a1, a2, b0, b1, b2);
}

void Biquad::setPoleZeroForm(const BiquadPoleState &bps) {
	setPoleZeroPair(bps);
	applyScale(bps.gain);
}

void Biquad::applyScale(float scale) {
	b[0] *= scale;
	b[1] *= scale;
	b[2] *= scale;
}

// Cascade----------------------------------------
void Cascade::setCascadeStorage(const Storage &storage) {
	numStages = storage.maxStages;
	maxStages = storage.maxStages;
	stages = storage.stageArray;
}

complex_t Cascade::response(float normalizedFrequency) const {
	if (normalizedFrequency > 0.5)
		Error_Handler();
	if (normalizedFrequency < 0.0)
		Error_Handler();
	float w = 2 * doublePi * normalizedFrequency;
	const complex_t czn1 = std::polar<float>(1.0f, -w);
	const complex_t czn2 = std::polar<float>(1.0f, -2 * w);
	complex_t ch(1);
	complex_t cbot(1);

	const Biquad *stage = stages;
	for (uint16_t i = numStages; --i >= 0; ++stage) {
		complex_t cb(1);
		complex_t ct(stage->getB0() / stage->getA0());
		ct = addmul(ct, stage->getB1() / stage->getA0(), czn1);
		ct = addmul(ct, stage->getB2() / stage->getA0(), czn2);
		cb = addmul(cb, stage->getA1() / stage->getA0(), czn1);
		cb = addmul(cb, stage->getA2() / stage->getA0(), czn2);
		ch *= ct;
		cbot *= cb;
	}

	return ch / cbot;
}

std::vector<PoleZeroPair> Cascade::getPoleZeros() const {
	std::vector < PoleZeroPair > vpz;
	vpz.reserve((unsigned long) numStages);

	const Biquad *stage = stages;
	for (int i = numStages; --i >= 0;) {
		BiquadPoleState bps(*stage++);
		vpz.push_back(bps);
	}

	return vpz;
}

void Cascade::applyScale(float scale) {
	if (numStages < 1)
		return;
	stages->applyScale(scale);
}

void Cascade::setLayout(const LayoutBase &proto) {
	const int numPoles = proto.getNumPoles();
	numStages = (numPoles + 1) / 2;
	if (numStages > maxStages)
		Error_Handler();

	Biquad *stage = stages;
	for (int i = 0; i < maxStages; ++i, ++stage)
		stage->setDefaults();

	stage = stages;
	for (int i = 0; i < numStages; ++i, ++stage)
		stage->setPoleZeroPair(proto[i]);

	applyScale(
			proto.getNormalGain()
					/ std::abs(response(proto.getNormalW() / (2 * doublePi))));
}

//Pole Filters==============================================
complex_t LowPassTransform::transform(complex_t c) {
	if (c == infinity())
		return complex_t(-1, 0);
	// frequency transform
	c = f * c;
	// bilinear low pass transform
	return (1.0f + c) / (1.0f - c);
}

LowPassTransform::LowPassTransform(float fc, LayoutBase &digital,
		LayoutBase const &analog) {

	if (!(fc < 0.5))
		Error_Handler();
	if (fc < 0.0)
		Error_Handler();

	digital.reset();

	// prewarp
	f = tan(doublePi * fc);

	const uint32_t numPoles = analog.getNumPoles();
	const uint32_t pairs = numPoles / 2;
	for (uint32_t i = 0; i < pairs; ++i) {
		const PoleZeroPair &pair = analog[i];
		digital.addPoleZeroConjugatePairs(transform(pair.poles.first),
				transform(pair.zeros.first));
	}

	if (numPoles & 1) {
		const PoleZeroPair &pair = analog[pairs];
		digital.add(transform(pair.poles.first), transform(pair.zeros.first));
	}

	digital.setNormal(analog.getNormalW(), analog.getNormalGain());
}
//-------------------------------
complex_t HighPassTransform::transform(complex_t c) {
	if (c == infinity())
		return complex_t(1, 0);
	// frequency transform
	c = f * c;
	// bilinear high pass transform
	return -(1.0f + c) / (1.0f - c);
}

HighPassTransform::HighPassTransform(float fc, LayoutBase &digital,
		LayoutBase const &analog) {
	if (!(fc < 0.5))
		Error_Handler();
	if (fc < 0.0)
		Error_Handler();

	digital.reset();

	// prewarp
	f = 1.0f / tan(doublePi * fc);

	const uint32_t numPoles = analog.getNumPoles();
	const uint32_t pairs = numPoles / 2;
	for (uint32_t i = 0; i < pairs; ++i) {
		const PoleZeroPair &pair = analog[i];
		digital.addPoleZeroConjugatePairs(transform(pair.poles.first),
				transform(pair.zeros.first));
	}

	if (numPoles & 1) {
		const PoleZeroPair &pair = analog[pairs];
		digital.add(transform(pair.poles.first), transform(pair.zeros.first));
	}

	digital.setNormal(doublePi - analog.getNormalW(), analog.getNormalGain());
}
//-------------------------------

ComplexPair BandPassTransform::transform(complex_t c) {
	if (c == infinity())
		return ComplexPair(-1, 1);

	c = (1.0f + c) / (1.0f - c); // bilinear

	complex_t v = 0.0f;
	v = addmul(v, 4 * (b2 * (a2 - 1) + 1), c);
	v += 8 * (b2 * (a2 - 1) - 1);
	v *= c;
	v += 4 * (b2 * (a2 - 1) + 1);
	v = std::sqrt(v);

	complex_t u = -v;
	u = addmul(u, ab_2, c);
	u += ab_2;

	v = addmul(v, ab_2, c);
	v += ab_2;

	complex_t d = 0;
	d = addmul(d, 2 * (b - 1), c) + 2 * (1 + b);

	return ComplexPair(u / d, v / d);
}

BandPassTransform::BandPassTransform(float fc, float fw, LayoutBase &digital,
		LayoutBase const &analog) {
	if (!(fc < 0.5f))
		Error_Handler();
	if (fc < 0.0f)
		Error_Handler();

	digital.reset();

	const float ww = 2.0f * doublePi * fw;

	// pre-calcs
	wc2 = 2.0f * doublePi * fc - (ww / 2.0f);
	wc = wc2 + ww;

	// what is this crap?
	if (wc2 < 1e-8)
		wc2 = 1e-8;
	if (wc > doublePi - 1e-8)
		wc = doublePi - 1e-8;

	a = cos((wc + wc2) * 0.5f) / cos((wc - wc2) * 0.5f);
	b = 1.0f / tan((wc - wc2) * 0.5f);
	a2 = a * a;
	b2 = b * b;
	ab = a * b;
	ab_2 = 2.0f * ab;

	const uint32_t numPoles = analog.getNumPoles();
	const uint32_t pairs = numPoles / 2;
	for (uint32_t i = 0; i < pairs; ++i) {
		const PoleZeroPair &pair = analog[i];
		ComplexPair p1 = transform(pair.poles.first);
		ComplexPair z1 = transform(pair.zeros.first);

		digital.addPoleZeroConjugatePairs(p1.first, z1.first);
		digital.addPoleZeroConjugatePairs(p1.second, z1.second);
	}

	if (numPoles & 1) {
		ComplexPair poles = transform(analog[pairs].poles.first);
		ComplexPair zeros = transform(analog[pairs].zeros.first);

		digital.add(poles, zeros);
	}

	double wn = analog.getNormalW();
	digital.setNormal(
			2.0f * atan(sqrt(tan((wc + wn) * 0.5f) * tan((wc2 + wn) * 0.5f))),
			analog.getNormalGain());
}
//-------------------------------
ComplexPair BandStopTransform::transform(complex_t c) {
	if (c == infinity())
		c = -1;
	else
		c = (1.0f + c) / (1.0f - c); // bilinear

	complex_t u(0);
	u = addmul(u, 4 * (b2 + a2 - 1.0f), c);
	u += 8.0f * (b2 - a2 + 1.0f);
	u *= c;
	u += 4.0f * (a2 + b2 - 1.0f);
	u = std::sqrt(u);

	complex_t v = u * -0.5f;
	v += a;
	v = addmul(v, -a, c);

	u *= 0.5f;
	u += a;
	u = addmul(u, -a, c);

	complex_t d(b + 1.0f);
	d = addmul(d, b - 1.0f, c);

	return ComplexPair(u / d, v / d);
}

BandStopTransform::BandStopTransform(float fc, float fw, LayoutBase &digital,
		LayoutBase const &analog) {
	if (!(fc < 0.5f))
		Error_Handler();
	if (fc < 0.0f)
		Error_Handler();

	digital.reset();

	const float ww = 2.0f * doublePi * fw;

	wc2 = 2.0f * doublePi * fc - (ww / 2.0f);
	wc = wc2 + ww;

	// this is crap
	if (wc2 < 1e-8)
		wc2 = 1e-8;
	if (wc > doublePi - 1e-8)
		wc = doublePi - 1e-8;

	a = cos((wc + wc2) * 0.5f) / cos((wc - wc2) * 0.5f);
	b = tan((wc - wc2) * 0.5f);
	a2 = a * a;
	b2 = b * b;

	const uint32_t numPoles = analog.getNumPoles();
	const uint32_t pairs = numPoles / 2;
	for (uint32_t i = 0; i < pairs; ++i) {
		const PoleZeroPair &pair = analog[i];
		ComplexPair p = transform(pair.poles.first);
		ComplexPair z = transform(pair.zeros.first);

		// trick to get the conjugate
		if (z.second == z.first)
			z.second = std::conj(z.first);

		digital.addPoleZeroConjugatePairs(p.first, z.first);
		digital.addPoleZeroConjugatePairs(p.second, z.second);
	}

	if (numPoles & 1) {
		ComplexPair poles = transform(analog[pairs].poles.first);
		ComplexPair zeros = transform(analog[pairs].zeros.first);

		digital.add(poles, zeros);
	}

	if (fc < 0.25)
		digital.setNormal(doublePi, analog.getNormalGain());
	else
		digital.setNormal(0, analog.getNormalGain());
}

} // namespace DSP

