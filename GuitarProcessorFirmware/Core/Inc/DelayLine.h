#ifndef INC_DELAYLINE_H_
#define INC_DELAYLINE_H_
#include <memory>
#include "main.h"

#define INLINE_DELAY_ACCESS
enum {
  TAP_MAIN = 0,
  TAP_OUT1,
  TAP_OUT2,
  TAP_OUT3,
  MAX_TAPS
};
#ifdef __cplusplus
class DelayLine{
private:
	uint16_t maxDelay;
	uint16_t length;
	float* data = nullptr;
	uint16_t mask = 0;
	bool useAXI = true;
public:
	uint16_t offsets[MAX_TAPS];
	DelayLine();
	~DelayLine();
	void init(uint16_t delay, bool useAXI = true);
	// set the length of a delay tap
	void setDelay(uint8_t tap, uint16_t delaySamples);
	// read/write access
#ifdef INLINE_DELAY_ACCESS
	inline float process(uint16_t cycle, float input){
		write(cycle, input);
		return read(TAP_MAIN, cycle);
	}
	inline void write(uint16_t cycle, float input){
		data[cycle & mask] = input;
	}
	inline float read(uint8_t tap, uint16_t cycle){
		return data[(cycle + offsets[tap]) & mask];
	}
#else 
	float read(uint8_t tap, uint16_t cycle);
	void write(uint16_t cycle, float input);
	float process(uint16_t cycle, float input);
#endif
};


#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_DELAYLINE_H_ */