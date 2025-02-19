/*
 * Butterworth.cpp
 *
 *  Created on: Feb 14, 2025
 *      Author: hayden
 */
#include "Butterworth.h"

namespace DSP {
namespace Butterworth {
// Analog Prototypes===================================================
AnalogLowPass::AnalogLowPass() :
		numPoles(-1) {
	setNormal(0, 1);
}

void AnalogLowPass::design(const int poles) {
	if (numPoles != poles) {
		numPoles = poles;

		reset();

		const float n2 = 2.0f * numPoles;
		const int pairs = numPoles / 2;
		for (int i = 0; i < pairs; ++i) {
			complex_t c = std::polar<float>(1.0f,
					doublePi_2 + (2.0f * i + 1.0f) * floatPi / n2);
			addPoleZeroConjugatePairs(c, infinity());
		}

		if (numPoles & 1)
			add(-1, infinity());
	}
}

//-----------
AnalogLowShelf::AnalogLowShelf() :
		numPoles(-1) {
	setNormal(floatPi, 1);
}

void AnalogLowShelf::design(int poles, float gain) {
	if (numPoles != poles || gainDb != gain) {
		numPoles = poles;
		gainDb = gain;

		reset();

		const float n2 = numPoles * 2.0f;
		const float g = std::pow(std::pow(10.0f, gainDb / 20.0f), 1.0f / n2);
		const float gp = -1.0f / g;
		const float gz = -g;

		const int pairs = numPoles / 2.0f;
		for (int i = 1; i <= pairs; ++i) {
			const float theta = floatPi * (0.5f - (float) (2 * i - 1) / n2);
			addPoleZeroConjugatePairs(std::polar(gp, theta),
					std::polar(gz, theta));
		}

		if (numPoles & 1)
			add(gp, gz);
	}
}

// filter bases===================================================
void LowPassBase::setup(int order, float cutoffFrequency) {
	analogProto.design(order);

	LowPassTransform(cutoffFrequency, digitalProto, analogProto);

	Cascade::setLayout(digitalProto);
}

void HighPassBase::setup(int order, float cutoffFrequency) {
	analogProto.design(order);

	HighPassTransform(cutoffFrequency, digitalProto, analogProto);

	Cascade::setLayout(digitalProto);
}

void BandPassBase::setup(int order, float centerFrequency,
		float widthFrequency) {
	analogProto.design(order);

	BandPassTransform(centerFrequency, widthFrequency, digitalProto,
			analogProto);

	Cascade::setLayout(digitalProto);
}

void BandStopBase::setup(int order, float centerFrequency,
		float widthFrequency) {
	analogProto.design(order);

	BandStopTransform(centerFrequency, widthFrequency, digitalProto,
			analogProto);

	Cascade::setLayout(digitalProto);
}

void LowShelfBase::setup(int order, float cutoffFrequency, float gainDb) {
	analogProto.design(order, gainDb);

	LowPassTransform(cutoffFrequency, digitalProto, analogProto);

	Cascade::setLayout(digitalProto);
}

void HighShelfBase::setup(int order, float cutoffFrequency, float gainDb) {
	analogProto.design(order, gainDb);

	HighPassTransform(cutoffFrequency, digitalProto, analogProto);

	Cascade::setLayout(digitalProto);
}

void BandShelfBase::setup(int order, float centerFrequency,
		float widthFrequency, float gainDb) {
	analogProto.design(order, gainDb);

	BandPassTransform(centerFrequency, widthFrequency, digitalProto,
			analogProto);

	// HACK!
	digitalProto.setNormal((centerFrequency < 0.25f) ? floatPi : 0, 1);

	Cascade::setLayout(digitalProto);
}

}
// namespace butterworth
}// namespace DSP

