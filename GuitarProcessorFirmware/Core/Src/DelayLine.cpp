#include "DelayLine.h"
#include <memory>
#include "AXISRAMPool.h"
#include "DTCMPool.h"

uint16_t neededBufSize(uint16_t delay) {
	uint16_t sigBits = 0;
	while (delay) {
		sigBits++;
		delay >>= 1;
	}
	return (1 << sigBits);
}

DelayLine::DelayLine() {
	for(uint32_t i = 0; i < MAX_TAPS; ++i){
		offsets[i] = 0;
	}
}

void DelayLine::init(uint16_t size, bool shouldUseASI) {
	useASI = shouldUseASI;
	maxDelay = size;
	length = neededBufSize(maxDelay);
	// allocate the buffer
	if(useASI){
		data = AXISRAMPool::alloc(length, true);
	} else {
		data = DTCMPool::alloc(length, true);
	}
	// initialize the bitmask
	mask = length - 1;
	// set the main delay tap to max
	offsets[TAP_MAIN] = mask + 1 - maxDelay;
}

void DelayLine::setDelay(uint8_t tap, uint16_t delay) {
	offsets[tap] = mask + 1 - delay;
}

DelayLine::~DelayLine() {
}
