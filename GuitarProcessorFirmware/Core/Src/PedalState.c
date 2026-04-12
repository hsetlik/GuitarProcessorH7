#include "PedalState.h"


void PedalState_init(pedal_state_t* ps){
    ps->algIdx = 0;
    ps->fxEngaged = true;
    ps->knobA = 0.0f;
    ps->knobB = 0.0f;
    ps->knobC = 0.0f;
}



uint8_t PedalState_getLedData(pedal_state_t* ps) {
   	/* In LSB-first order the LEDs are wired like:
	 * 0: Alg 5
	 * 1: Alg 4
	 * 2: Alg 3
	 * 3: Alg 2
	 * 4: Alg 1
	 * 5: Alg 0
	 * 6: Tempo
	 * 7: Bypass
	 * */
	uint8_t byte = 0x00;
	// switch the appropriate alg. LED on
	uint8_t mask = 0x01 << (5 - ps->algIdx);
	byte = byte | mask;
	if(ps->fxEngaged){
		byte = byte | (0x01 << 7);
	}
	return byte; 
}