/*
 * FastSqrt.h
 *
 *  Created on: Feb 18, 2025
 *      Author: hayden
 */

#ifndef INC_FASTSQRT_H_
#define INC_FASTSQRT_H_

#ifdef __cplusplus

#include <bit>
#include <cstdint>
#include <limits>

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
static constexpr float fastInvSqrt(float x) noexcept
{
	constexpr float threeHalves = 1.5f;
	float x2 = x * 0.5f;
	// it's called we do a little bit hacking
	long i = *(long*)&x;
	i = 0x05F3759DF - (i >> 1);
	x = *(float*)&i;
	x = x * (threeHalves - (x2 * x * x));
	return x;


}

static constexpr float fastSqrt(float x)
{
    return fastInvSqrt(x) * x;

}

#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_FASTSQRT_H_ */
