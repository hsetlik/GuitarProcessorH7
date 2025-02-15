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
namespace DSP {
namespace Butterworth {
// analog prototypes-----------------------------------------

// Analog low pass prototype (s-plane)
class AnalogLowPass: public LayoutBase {
public:
	AnalogLowPass();
	void design(const int poles);
private:
	int numPoles = 0;
};

// Analog low shelf prototype (s-plane)
class AnalogLowShelf: public LayoutBase {
public:
	AnalogLowShelf();
	void design(const int poles, float gain);
private:
	int numPoles = 0;
	float gainDb = 0.0f;
};

// filter bases----------------------------------------
struct LowPassBase: PoleFilterBase<AnalogLowPass> {
	void setup(int order, float cutoffFrequency);
};

struct HighPassBase: PoleFilterBase<AnalogLowPass> {
	void setup(int order, float cutoffFrequency);
};

struct BandPassBase: PoleFilterBase<AnalogLowPass> {
	void setup(int order, float centerFreq, float widthFreq);
};

struct BandStopBase: PoleFilterBase<AnalogLowPass> {
	void setup(int order, float centerFreq, float widthFreq);
};

struct LowShelfBase: PoleFilterBase<AnalogLowShelf> {
	void setup(int order, float centerFreq, float gainDb);
};

struct HighShelfBase: PoleFilterBase<AnalogLowShelf> {
	void setup(int order, float centerFreq, float gainDb);
};

struct BandShelfBase: PoleFilterBase<AnalogLowShelf> {
	void setup(int order, float centerFreq, float widthFreq, float gainDb);
};

// actually useful filter
template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct LowPass: PoleFilter<LowPassBase, FilterOrder> {
	/**
	 * Calculates the coefficients
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 **/
	void setup(float sampleRate, float cutoffFrequency) {
		LowPassBase::setup(FilterOrder, cutoffFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 **/
	void setup(int reqOrder, float sampleRate, float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowPassBase::setup(reqOrder, cutoffFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 **/
	void setupN(float cutoffFrequency) {
		LowPassBase::setup(FilterOrder, cutoffFrequency);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 **/
	void setupN(int reqOrder, float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowPassBase::setup(reqOrder, cutoffFrequency);
	}
};

template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct HighPass: PoleFilter<HighPassBase, FilterOrder> {
	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff frequency
	 **/
	void setup(float sampleRate, float cutoffFrequency) {
		HighPassBase::setup(FilterOrder, cutoffFrequency / sampleRate);
	}
	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff frequency
	 **/
	void setup(int reqOrder, float sampleRate, float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		HighPassBase::setup(reqOrder, cutoffFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 **/
	void setupN(float cutoffFrequency) {
		HighPassBase::setup(FilterOrder, cutoffFrequency);
	}
	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 **/
	void setupN(int reqOrder, float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		HighPassBase::setup(reqOrder, cutoffFrequency);
	}
};

/**
 * Butterworth  Bandpass filter.
 * \param FilterOrder  Reserves memory for a filter of the order FilterOrder
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 */
template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct BandPass: PoleFilter<BandPassBase, FilterOrder, FilterOrder * 2> {
	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Centre frequency of the bandpass
	 * \param widthFrequency Width of the bandpass
	 **/
	void setup(float sampleRate, float centerFrequency, float widthFrequency) {
		BandPassBase::setup(FilterOrder, centerFrequency / sampleRate,
				widthFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Centre frequency of the bandpass
	 * \param widthFrequency Width of the bandpass
	 **/
	void setup(int reqOrder, float sampleRate, float centerFrequency,
			float widthFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		BandPassBase::setup(reqOrder, centerFrequency / sampleRate,
				widthFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param centerFrequency Normalised centre frequency (0..1/2) of the bandpass
	 * \param widthFrequency Width of the bandpass in normalised freq
	 **/
	void setupN(float centerFrequency, float widthFrequency) {
		BandPassBase::setup(FilterOrder, centerFrequency, widthFrequency);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param centerFrequency Normalised centre frequency (0..1/2) of the bandpass
	 * \param widthFrequency Width of the bandpass in normalised freq
	 **/
	void setupN(int reqOrder, float centerFrequency, float widthFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		BandPassBase::setup(reqOrder, centerFrequency, widthFrequency);
	}
};

// shelf filters-----------------------------------

template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct LowShelf: PoleFilter<LowShelfBase, FilterOrder> {
	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setup(float sampleRate, float cutoffFrequency, float gainDb) {
		LowShelfBase::setup(FilterOrder, cutoffFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setup(int reqOrder, float sampleRate, float cutoffFrequency,
			float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowShelfBase::setup(reqOrder, cutoffFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setupN(float cutoffFrequency, float gainDb) {
		LowShelfBase::setup(FilterOrder, cutoffFrequency, gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setupN(int reqOrder, float cutoffFrequency, float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowShelfBase::setup(reqOrder, cutoffFrequency, gainDb);
	}

};

/**
 * Butterworth high shelf filter. Above the cutoff the filter has
 * a specified gain and below it has 0 dB.
 * \param FilterOrder Reserves memory for a filter of the order FilterOrder
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 **/
template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct HighShelf: PoleFilter<HighShelfBase, FilterOrder> {
	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setup(float sampleRate, float cutoffFrequency, float gainDb) {
		HighShelfBase::setup(FilterOrder, cutoffFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param cutoffFrequency Cutoff
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setup(int reqOrder, float sampleRate, float cutoffFrequency,
			float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		HighShelfBase::setup(reqOrder, cutoffFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setupN(float cutoffFrequency, float gainDb) {
		HighShelfBase::setup(FilterOrder, cutoffFrequency, gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	 * \param gainDb Gain in dB of the filter in the passband
	 **/
	void setupN(int reqOrder, float cutoffFrequency, float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		HighShelfBase::setup(reqOrder, cutoffFrequency, gainDb);
	}
};

/**
 * Butterworth Bandshelf filter: it is a bandpass filter which amplifies at a specified
 * gain in dB the frequencies in the passband.
 * \param FilterOrder Reserves memory for a filter of the order FilterOrder
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 */
template<int FilterOrder = DEFAULT_FILTER_ORDER>
struct BandShelf: PoleFilter<BandShelfBase, FilterOrder, FilterOrder * 2> {
	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Centre frequency of the passband
	 * \param widthFrequency Width of the passband
	 * \param gainDb The gain in the passband
	 **/
	void setup(float sampleRate, float centerFrequency, float widthFrequency,
			float gainDb) {
		BandShelfBase::setup(FilterOrder, centerFrequency / sampleRate,
				widthFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Centre frequency of the passband
	 * \param widthFrequency Width of the passband
	 * \param gainDb The gain in the passband
	 **/
	void setup(int reqOrder, float sampleRate, float centerFrequency,
			float widthFrequency, float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		BandShelfBase::setup(reqOrder, centerFrequency / sampleRate,
				widthFrequency / sampleRate, gainDb);
	}

	/**
	 * Calculates the coefficients with the filter order provided by the instantiation
	 * \param centerFrequency Normalised centre frequency (0..1/2) of the passband
	 * \param widthFrequency Width of the passband
	 * \param gainDb The gain in the passband
	 **/
	void setupN(float centerFrequency, float widthFrequency, float gainDb) {
		BandShelfBase::setup(FilterOrder, centerFrequency, widthFrequency,
				gainDb);
	}

	/**
	 * Calculates the coefficients
	 * \param reqOrder The actual order which can be less than the instantiated one
	 * \param centerFrequency Normalised centre frequency (0..1/2) of the passband
	 * \param widthFrequency Width of the passband
	 * \param gainDb The gain in the passband
	 **/
	void setupN(int reqOrder, float centerFrequency, float widthFrequency,
			float gainDb) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		BandShelfBase::setup(reqOrder, centerFrequency, widthFrequency, gainDb);
	}
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
