/*
 * DSPMath.h
 *
 *  Created on: Feb 14, 2025
 *      Author: hayden
 */

#ifndef INC_DSPMATH_H_
#define INC_DSPMATH_H_

#ifdef __cplusplus
#include <cassert>
#include <cfloat>
#include <cmath>
#include <complex>
#include <cstring>
#include <string>
#include <limits>
#include <vector>
#include <complex>


namespace DSP {
// IIR math helpers
const float doublePi	=3.1415926535897932384626433832795028841971;
const float doublePi_2	=1.5707963267948966192313216916397514420986;
const float doubleLn2  =0.69314718055994530941723212145818;
const float doubleLn10	=2.3025850929940456840179914546844;

typedef std::complex<float> complex_t;
typedef std::pair<complex_t, complex_t> complex_pair_t;

inline const complex_t infinity()
{
  return complex_t (std::numeric_limits<float>::infinity());
}

template <typename Ty, typename To>
inline std::complex<Ty> addmul (const std::complex<Ty>& c,
                                Ty v,
                                const std::complex<To>& c1)
{
  return std::complex <Ty> (
    c.real() + v * c1.real(), c.imag() + v * c1.imag());
}

template <typename Ty>
inline Ty asinh (Ty x)
{
  return log (x + std::sqrt (x * x + 1 ));
}

template <typename Ty>
inline bool is_nan (Ty v)
{
  return !(v == v);
}

template <>
inline bool is_nan<complex_t> (complex_t v)
{
  return DSP::is_nan (v.real()) || DSP::is_nan (v.imag());
}

// Types for complex plane stuff
/**
 * A conjugate or real pair
 **/
	struct ComplexPair : complex_pair_t
	{
		ComplexPair() = default;

		explicit ComplexPair (const complex_t& c1)
			: complex_pair_t (c1, 0.0f)
		{
			if (!isReal())
				Error_Handler();
		}

		ComplexPair (const complex_t& c1,
			     const complex_t& c2)
			: complex_pair_t (c1, c2)
		{
		}

		bool isReal () const
		{
			return first.imag() == 0 && second.imag() == 0;
		}

		/**
		 * Returns true if this is either a conjugate pair,
		 * or a pair of reals where neither is zero.
		 */
		bool isMatchedPair () const
		{
			if (first.imag() != 0)
				return second == std::conj (first);
			else
				return second.imag () == 0 &&
					second.real () != 0 &&
					first.real () != 0;
		}

		bool is_nan () const
		{
			return DSP::is_nan (first) || DSP::is_nan (second);
		}
	};


/**
 * A pair of pole/zeros. This fits in a biquad (but is missing the gain)
 **/
	struct PoleZeroPair
	{
		ComplexPair poles = ComplexPair();
		ComplexPair zeros = ComplexPair();

		PoleZeroPair () = default;

		// single pole/zero
		PoleZeroPair (const complex_t& p, const complex_t& z)
			: poles (p), zeros (z)
		{
		}

		// pole/zero pair
		PoleZeroPair (const complex_t& p1, const complex_t& z1,
			      const complex_t& p2, const complex_t& z2)
			: poles (p1, p2)
			, zeros (z1, z2)
		{
		}

		bool isSinglePole () const
		{
			return (poles.second == std::complex<float>(0.0f)) && (zeros.second == std::complex<float>(0.0f));
		}

		bool is_nan () const
		{
			return poles.is_nan() || zeros.is_nan();
		}
	};



}



#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_DSPMATH_H_ */
