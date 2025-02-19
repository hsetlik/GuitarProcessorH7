/*
 * IIR.h
 *
 *  Created on: Feb 13, 2025
 *      Author: hayden
 */

#ifndef INC_IIR_H_
#define INC_IIR_H_
#include "main.h"

/* The code in this file is based on the Biquad implementations
 * in the IIR1 library: https://github.com/berndporr/iir1
 *
 * All the shared filtering stuff is in this translation unit,
 * each filter topology will get its own .h/.cpp file
 * */
#ifdef __cplusplus
#define DEFAULT_FILTER_ORDER 4

#include "DSPMath.h"
namespace DSP {
struct BiquadPoleState;
class Biquad {
	float a[3] = { 1.0f, 0.0f, 0.0f };
	float b[3] = { 1.0f, 0.0f, 0.0f };
	float x[2] = { 0.0f, 0.0f };
	float y[2] = { 0.0f, 0.0f };
public:
	Biquad() {
	}
	// main filter callback(s)
	float filter_DirectFormI(float input);
	// return to defaults
	void setDefaults() {
		a[0] = 1.0f;
		a[1] = 0.0f;
		a[2] = 0.0f;
		b[0] = 1.0f;
		b[1] = 0.0f;
		b[2] = 0.0f;
	}
	// reset the feedback buffer but leave the coeffs
	void fbReset() {
		x[0] = 0.0f;
		x[1] = 0.0f;
		y[0] = 0.0f;
		y[1] = 0.0f;
	}

	// get the complex response for the given normalized frequency
	complex_t response(float normFrequency) const;
	std::vector<PoleZeroPair> getPoleZeros() const;
	// setter / getters for the biquad coeffs
	float getA0() const {
		return a[0];
	}
	float getA1() const {
		return a[1] * a[0];
	}
	float getA2() const {
		return a[2] * a[0];
	}
	float getB0() const {
		return b[0] * a[0];
	}
	float getB1() const {
		return b[1] * a[0];
	}
	float getB2() const {
		return b[2] * a[0];
	}
	void setCoeffs(float a0, float a1, float a2, float b0, float b1, float b2);
	// pole/zero setup stuff

	/**
	 * Sets one (real) pole and zero. Throws exception if imaginary components.
	 **/
	void setOnePole(complex_t pole, complex_t zero);

	/**
	 * Sets two poles/zoes as a pair. Needs to be complex conjugate.
	 **/
	void setTwoPole(complex_t pole1, complex_t zero1, complex_t pole2,
			complex_t zero2);

	/**
	 * Sets a complex conjugate pair
	 **/
	void setPoleZeroPair(const PoleZeroPair &pair) {
		if (pair.isSinglePole())
			setOnePole(pair.poles.first, pair.zeros.first);
		else
			setTwoPole(pair.poles.first, pair.zeros.first, pair.poles.second,
					pair.zeros.second);
	}

	// set the gain of the filter
	void applyScale(float scale);

	void setPoleZeroForm(const BiquadPoleState &bps);
};
//--------------------------------------
/**
 * Expresses a biquad as a pair of pole/zeros, with gain
 * values so that the coefficients can be reconstructed precisely.
 **/
struct BiquadPoleState: PoleZeroPair {
	BiquadPoleState() = default;

	explicit BiquadPoleState(const Biquad &s);

	float gain = 1.0;
};

//--------------------------------------
// structure for storing and computing a series of biquads
class Cascade {
private:
	uint16_t numStages = 0;
	uint16_t maxStages = 0;
	Biquad *stages = nullptr;
public:
	Cascade() = default;
	struct Storage {
		Storage() = delete;
		Storage(const uint16_t maxNumBiquads,
				Biquad *const biquadArray) :
				maxStages(maxNumBiquads), stageArray(
						biquadArray) {
		}
		const uint16_t maxStages = 0;
		Biquad *const stageArray = nullptr;
	};
	// store a set of Biquads
	// number of biquads stored
	uint16_t getNumStages() const {
		return numStages;
	}
	// get a specific Biquad
	Biquad& operator[](uint16_t i) {
		return stages[i];
	}
	//-------------
	complex_t response(float normalizedFrequency) const;
	std::vector<PoleZeroPair> getPoleZeros() const;
	void setCascadeStorage(const Storage &storage);
	void applyScale(float scale);
	void setLayout(const LayoutBase &proto);

};

/**
 * Storage for Cascade: This holds a chain of 2nd order filters
 * with its coefficients.
 **/
template<uint16_t MaxStages>
class CascadeStages {

public:
	CascadeStages() = default;

public:
	/**
	 * Resets all biquads (i.e. the delay lines but not the coefficients)
	 **/
	void reset() {
		for (auto &stage : stages)
			stage.fbReset();
	}

public:
	/**
	 * Sets the coefficients of the whole chain of
	 * biquads.
	 * \param sosCoefficients 2D array in Python style sos ordering: 0-2: FIR, 3-5: IIR coeff.
	 **/
	void setup(const double (&sosCoefficients)[MaxStages][6]) {
		for (uint16_t i = 0; i < MaxStages; i++) {
			stages[i].setCoefficients(sosCoefficients[i][3],
					sosCoefficients[i][4], sosCoefficients[i][5],
					sosCoefficients[i][0], sosCoefficients[i][1],
					sosCoefficients[i][2]);
		}
	}

public:
	/**
	 * Filters one sample through the whole chain of biquads and return the result
	 * \param in Sample to be filtered
	 * \return filtered sample
	 **/
	float filter(float in) {
		float out = in;
		for (Biquad &stage : stages)
			out = stage.filter_DirectFormI(out);
		return out;
	}

	/**
	 * Returns the coefficients of the entire Biquad chain
	 **/
	const Cascade::Storage getCascadeStorage() {
		const Cascade::Storage s(MaxStages, stages);
		return s;
	}

private:
	Biquad stages[MaxStages] = { };
};

// Pole Filter--------------------------------------------------
/***
 * Base for filters designed via algorithmic placement of poles and zeros.
 *
 * Typically, the filter is first designed as a half-band low pass or
 * low shelf analog filter (s-plane). Then, using a transformation such
 * as the ones from Constantinides, the poles and zeros of the analog filter
 * are calculated in the z-plane.
 *
 ***/

/**
 * Factored implementations to reduce template instantiations
 **/
class PoleFilterBase2: public Cascade {
public:
	// This gets the poles/zeros directly from the digital
	// prototype. It is used to double check the correctness
	// of the recovery of pole/zeros from biquad coefficients.
	//
	// It can also be used to accelerate the interpolation
	// of pole/zeros for parameter modulation, since a pole
	// filter already has them calculated

	PoleFilterBase2() = default;

	std::vector<PoleZeroPair> getPoleZeros() const {
		std::vector<PoleZeroPair> vpz;
		const uint32_t pairs = (digitalProto.getNumPoles() + 1) / 2;
		for (uint32_t i = 0; i < pairs; ++i)
			vpz.push_back(digitalProto[i]);
		return vpz;
	}

protected:
	LayoutBase digitalProto = { };
};

/**
 * Serves a container to hold the analog prototype
 * and the digital pole/zero layout.
 **/
template<class AnalogPrototype>
class PoleFilterBase: public PoleFilterBase2 {
protected:
	void setPrototypeStorage(const LayoutBase &analogStorage,
			const LayoutBase &digitalStorage) {
		analogProto.setStorage(analogStorage);
		digitalProto = digitalStorage;
	}

protected:
	AnalogPrototype analogProto = { };
};

/**
 * Storage for pole filters
 **/
template<class BaseClass, int MaxAnalogPoles, int MaxDigitalPoles =
		MaxAnalogPoles>
struct PoleFilter: BaseClass, CascadeStages<(MaxDigitalPoles + 1) / 2> {
public:
	PoleFilter() {
		// This glues together the factored base classes
		// with the templatized storage classes.
		BaseClass::setCascadeStorage(CascadeStages<(MaxDigitalPoles + 1) / 2>::getCascadeStorage());
		BaseClass::setPrototypeStorage(analogStorage, digitalStorage);
		CascadeStages<(MaxDigitalPoles + 1) / 2>::reset();
	}

	PoleFilter(const PoleFilter&) = default;

	PoleFilter& operator=(const PoleFilter&) {
		// Reset the filter state when copied for now
		CascadeStages<(MaxDigitalPoles + 1) / 2>::reset();
		return *this;
	}

private:
	Layout<MaxAnalogPoles> analogStorage = { };
	Layout<MaxDigitalPoles> digitalStorage = { };
};

//------------------------------------------------------------------------------

/**
 * s-plane to z-plane transforms
 *
 * For pole filters, an analog prototype is created via placement of
 * poles and zeros in the s-plane. The analog prototype is either
 * a halfband low pass or a halfband low shelf. The poles, zeros,
 * and normalization parameters are transformed into the z-plane
 * using variants of the bilinear transformation.
 *
 **/

/**
 * low pass to low pass
 **/
class LowPassTransform {
public:
	LowPassTransform(float fc, LayoutBase &digital, LayoutBase const &analog);
private:
	complex_t transform(complex_t c);
	float f = 0.0;
};

/**
 * low pass to high pass
 **/
class HighPassTransform {
public:
	HighPassTransform(float fc, LayoutBase &digital, LayoutBase const &analog);

private:
	complex_t transform(complex_t c);
	float f = 0.0;
};
/**
 * low pass to band pass transform
 **/
class BandPassTransform {

public:
	BandPassTransform(float fc, float fw, LayoutBase &digital,
			LayoutBase const &analog);

private:
	ComplexPair transform(complex_t c);

	float wc = 0.0;
	float wc2 = 0.0;
	float a = 0.0;
	float b = 0.0;
	float a2 = 0.0;
	float b2 = 0.0;
	float ab = 0.0;
	float ab_2 = 0.0;
};
/**
 * low pass to band stop transform
 **/
class BandStopTransform {
public:
	BandStopTransform(float fc, float fw, LayoutBase &digital,
			LayoutBase const &analog);

private:
	ComplexPair transform(complex_t c);

	float wc = 0.0;
	float wc2 = 0.0;
	float a = 0.0;
	float b = 0.0;
	float a2 = 0.0;
	float b2 = 0.0;
};

//------------------------------------------------------------------------------

//======================================================
}// namespace DSP

#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_IIR_H_ */
