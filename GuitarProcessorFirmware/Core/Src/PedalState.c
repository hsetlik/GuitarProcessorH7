#include "PedalState.h"
#include <complex.h>


void PedalState_init(pedal_state_t* ps){
    ps->algIdx = 0;
    ps->fxEngaged = true;
    ps->knobA = 0.0f;
    ps->knobB = 0.0f;
    ps->knobC = 0.0f;
    ps->knobD = 0.0f;
	ps->expButton = false;
}



uint8_t PedalState_getLedData(pedal_state_t* ps) {
   	/* In LSB-first order the LEDs are wired like:
	 * 0: Tempo
	 * 1: Alg 5
	 * 2: Alg 4
	 * 3: Alg 3
	 * 4: Alg 2
	 * 5: Alg 1
	 * 6: Alg 0
	 * 7: Bypass
	 * */
	uint8_t byte = 0b10000000;
	// switch the appropriate alg. LED on
	uint8_t mask = 0x01 << (ps->algIdx + 1);
	byte = byte | mask;
	if(ps->fxEngaged){
		byte = byte | (0b00000001);
	}
	return byte; 
}