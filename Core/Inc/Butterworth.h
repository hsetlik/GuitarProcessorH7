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
// analog prototypes-----------------------------------------

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

// filter bases----------------------------------------
struct LowPassBase : PoleFilterBase<AnalogLowPass>{
	void setup(int order, float cutoffFrequency);
};

struct HighPassBase : PoleFilterBase<AnalogLowPass>{
	void setup(int order, float cutoffFrequency);
};

struct BandPassBase : PoleFilterBase<AnalogLowPass>{
	void setup(int order, float centerFreq, float widthFreq);
};

struct BandStopBase : PoleFilterBase<AnalogLowPass>{
	void setup(int order, float centerFreq, float widthFreq);
};

struct LowShelfBase : PoleFilterBase<AnalogLowShelf>{
	void setup(int order, float centerFreq, float gainDb);
};

struct HighShelfBase : PoleFilterBase<AnalogLowShelf>{
	void setup(int order, float centerFreq, float gainDb);
};

struct BandShelfBase : PoleFilterBase<AnalogLowShelf>{
	void setup(int order, float centerFreq, float widthFreq, float gainDb);
};

// actually useful filter
template <int FilterOrder = DEFAULT_FILTER_ORDER>
struct LowPass : PoleFilter<LowPassBase, FilterOrder>{
	/**
	 * Calculates the coefficients
         * \param sampleRate Sampling rate
         * \param cutoffFrequency Cutoff
         **/
	void setup (float sampleRate,
		    float cutoffFrequency) {
		LowPassBase::setup (FilterOrder,
				    cutoffFrequency / sampleRate);
	}

	/**
	 * Calculates the coefficients
         * \param reqOrder The actual order which can be less than the instantiated one
         * \param sampleRate Sampling rate
         * \param cutoffFrequency Cutoff
         **/
	void setup (int reqOrder,
		    float sampleRate,
		    float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowPassBase::setup (reqOrder,
				    cutoffFrequency / sampleRate);
	}


	/**
	 * Calculates the coefficients
         * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
         **/
	void setupN(float cutoffFrequency) {
		LowPassBase::setup (FilterOrder,
				    cutoffFrequency);
	}

	/**
	 * Calculates the coefficients
         * \param reqOrder The actual order which can be less than the instantiated one
         * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
         **/
	void setupN(int reqOrder,
		    float cutoffFrequency) {
		if (reqOrder > FilterOrder)
			Error_Handler();
		LowPassBase::setup (reqOrder,
				    cutoffFrequency);
	}
};


template <int FilterOrder = DEFAULT_FILTER_ORDER>
struct HighPass : PoleFilter<HighPassBase, FilterOrder>{
	/**
		 * Calculates the coefficients with the filter order provided by the instantiation
	         * \param sampleRate Sampling rate
	         * \param cutoffFrequency Cutoff frequency
	         **/
		void setup (float sampleRate,
			    float cutoffFrequency) {
			HighPassBase::setup (FilterOrder,
					     cutoffFrequency / sampleRate);
		}
		/**
		 * Calculates the coefficients
	         * \param reqOrder The actual order which can be less than the instantiated one
	         * \param sampleRate Sampling rate
	         * \param cutoffFrequency Cutoff frequency
	         **/
		void setup (int reqOrder,
			    float sampleRate,
			    float cutoffFrequency) {
			if (reqOrder > FilterOrder)
				Error_Handler();
			HighPassBase::setup (reqOrder,
					     cutoffFrequency / sampleRate);
		}


		/**
		 * Calculates the coefficients with the filter order provided by the instantiation
	         * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	         **/
		void setupN(float cutoffFrequency) {
			HighPassBase::setup (FilterOrder,
					     cutoffFrequency);
		}
		/**
		 * Calculates the coefficients
	         * \param reqOrder The actual order which can be less than the instantiated one
	         * \param cutoffFrequency Normalised cutoff frequency (0..1/2)
	         **/
		void setupN(int reqOrder,
			    float cutoffFrequency) {
			if (reqOrder > FilterOrder)
				Error_Handler();
			HighPassBase::setup (reqOrder,
					     cutoffFrequency);
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
