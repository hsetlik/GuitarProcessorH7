/*
 * IIR.h
 *
 *  Created on: Feb 13, 2025
 *      Author: hayden
 */

#ifndef INC_IIR_H_
#define INC_IIR_H_
#include "main.h"

#define DEFAULT_IIR_ORDER 4



#ifdef __cplusplus

#include "DSPMath.h"
namespace DSP {
// Biquad IIR class based on the IIR1 library: https://github.com/berndporr/iir1
struct BiquadPoleState;
class Biquad {
	float a[3] = {1.0f, 0.0f, 0.0f};
	float b[3] = {1.0f, 0.0f, 0.0f};
	float x[2] = {0.0f, 0.0f};
	float y[2] = {0.0f, 0.0f};
public:
	Biquad(){
	}
	// main filter callback(s)
	float filter_DirectFormI(float input);

	// get the complex response for the given normalized frequency
	complex_t response(float normFrequency) const;
	std::vector<PoleZeroPair> getPoleZeros() const;
	// setter / getters for the biquad coeffs
	float getA0() const { return a[0]; }
	float getA1() const { return a[1] * a[0]; }
	float getA2() const { return a[2] * a[0]; }
	float getB0() const { return b[0] * a[0]; }
	float getB1() const { return b[1] * a[0]; }
	float getB2() const { return b[2] * a[0]; }
	void setCoeffs(float a0, float a1, float a2,
			float b0, float b1, float b2);
	// pole/zero setup stuff

	/**
	 * Sets one (real) pole and zero. Throws exception if imaginary components.
	 **/
	void setOnePole (complex_t pole, complex_t zero);

	/**
	 * Sets two poles/zoes as a pair. Needs to be complex conjugate.
	 **/
	void setTwoPole (complex_t pole1, complex_t zero1,
			 complex_t pole2, complex_t zero2);

	/**
	 * Sets a complex conjugate pair
	 **/
	void setPoleZeroPair (const PoleZeroPair& pair)
	{
		if (pair.isSinglePole ())
			setOnePole (pair.poles.first, pair.zeros.first);
		else
			setTwoPole (pair.poles.first, pair.zeros.first,
				    pair.poles.second, pair.zeros.second);
	}

	// set the gain of the filter
	void applyScale(float scale);

	void setPoleZeroForm (const BiquadPoleState& bps);
};
//--------------------------------------
/**
 * Expresses a biquad as a pair of pole/zeros, with gain
 * values so that the coefficients can be reconstructed precisely.
 **/
	struct BiquadPoleState : PoleZeroPair
	{
		BiquadPoleState () = default;

		explicit BiquadPoleState (const Biquad& s);

		float gain = 1.0;
	};

//--------------------------------------
	// structure for storing and computing a series of biquads
class Cascade {
private:
		uint16_t numStages = 0;
		uint16_t maxStages = 0;
		Biquad* stages = nullptr;
public:
		Cascade()=default;
	     struct Storage
	        {
		    Storage() = delete;
		    Storage(const uint16_t maxNumBiquads, Biquad* const biquadArray) : maxStages(maxNumBiquads), stageArray(biquadArray) {}
		    const uint16_t maxStages = 0;
		    Biquad* const stageArray = nullptr;
	        };
	     // store a set of Biquads
	     // number of biquads stored
	     uint16_t getNumStages() const {
	    	 return numStages;
	     }
	     // get a specific Biquad
	     Biquad& operator[] (uint16_t i){
	    	 return stages[i];
	     }
	     //-------------
	     complex_t response(float normalizedFrequency) const;
	     std::vector<PoleZeroPair> getPoleZeros() const;
	     void setStorage(const Storage& storage);

	};

//======================================================
} // namespace DSP



class IIRFilter {
private:
	const uint16_t length;
	float* aCoeffs;
	float* bCoeffs;
	float* inBuf;
	float* outBuf;
public:
	IIRFilter(uint16_t size, float* aBuf, float* bBuf);
	~IIRFilter();
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

#endif /* INC_IIR_H_ */
