/*
 * Dattorro.cpp
 *
 *  Created on: Feb 16, 2025
 *      Author: hayden
 */
#include "Dattorro.h"
#include <algorithm>
// filter helpers
float LP_process(float* out, float in, float freq) {
	*out += (in - *out) * freq;
	return *out;
}
float AP_process(DelayLine *dl, uint16_t cycle, float gain, float in) {
	float delayed = dl->read(0, cycle);
	in += delayed * -gain;
	dl->write(cycle, in);
	return delayed + in * gain;
}

Dattorro::Dattorro() {
	// Initialize the delay buffers w the numbers from Dattorro's paper
	preDelay.init(MAX_PREDELAY);
	// input diffusion delays
	inDiffusion[0].init(142);
	inDiffusion[1].init(107);
	inDiffusion[2].init(379);
	inDiffusion[3].init(277);
	// left tank----------------------------
	decayDiffusion1[0].init(672);

	preDampingDelay[0].init(4453);
	preDampingDelay[0].setDelay(1, 353);
	preDampingDelay[0].setDelay(2, 3627);
	preDampingDelay[0].setDelay(3, 1990);

	decayDiffusion2[0].init(1800);
	decayDiffusion2[0].setDelay(1, 187);
	decayDiffusion2[0].setDelay(2, 1228);

	postDampingDelay[0].init(3720);
	postDampingDelay[0].setDelay(1, 1066);
	postDampingDelay[0].setDelay(2, 2673);

	// right tank----------------------------
	decayDiffusion1[1].init(908);

	preDampingDelay[1].init(4217);
	preDampingDelay[1].setDelay(1, 266);
	preDampingDelay[1].setDelay(2, 2974);
	preDampingDelay[1].setDelay(3, 2111);

	decayDiffusion2[1].init(2656);
	decayDiffusion2[1].setDelay(1, 335);
	decayDiffusion2[1].setDelay(2, 1913);

	postDampingDelay[1].init(3163);
	postDampingDelay[1].setDelay(1, 121);
	postDampingDelay[1].setDelay(2, 1996);

	// settings
	uint16_t preDel = MAX_PREDELAY / 10;
	preDelay.setDelay(0, preDel);
	preFilterAmt = 0.85f;
	inputDiff1Amt = 0.75f;
	inputDiff2Amt = 0.625f;
	decayAmt = 0.75f;
	decayDiff1Amt = 0.70f;
	dampingAmt = 0.95f;

}

// the main algorithm
void Dattorro::processIn(float input) {
	// modulate decayDiffusion1 for both tanks
	if ((t & 0x07ff) == 0) {
		if (t < (1 << 15)) {
			decayDiffusion1[0].offsets[0]--;
			decayDiffusion1[1].offsets[0]--;
		} else {
			decayDiffusion1[0].offsets[0]++;
			decayDiffusion1[1].offsets[0]++;
		}
	}
	// pre-delay
	float x = preDelay.process(t, input);
	// pre-filter
	x = LP_process(&preFilter, x, preFilterAmt);
	// input diffusion
	x = AP_process(&inDiffusion[0], t, inputDiff1Amt, x);
	x = AP_process(&inDiffusion[1], t, inputDiff1Amt, x);
	x = AP_process(&inDiffusion[2], t, inputDiff2Amt, x);
	x = AP_process(&inDiffusion[3], t, inputDiff2Amt, x);
	// process the tanks
	float x1;
	for(uint16_t i = 0; i < 2; i++){
		// cross feedback
		x1 = x + postDampingDelay[1 - i].read(0, t) * decayAmt;
		// back to this half ot the tank
		x1 = AP_process(&decayDiffusion1[i], t, decayDiff1Amt, x1);
		x1 = preDampingDelay[i].process(t, x1);
		x1 = LP_process(&damping[i], x1, dampingAmt);
		x1 *= decayAmt;
		AP_process(&decayDiffusion2[i], t, decayDiff2Amt, x1);
		postDampingDelay[i].write(t, x1);
	}
	// increment t
	t++;
}

float Dattorro::getLeft(){
	float a = preDampingDelay[1].read(1, t);
	a += preDampingDelay[1].read(2, t);
	a -= decayDiffusion2[1].read(2, t);
	a += postDampingDelay[1].read(2, t);
	a -= preDampingDelay[0].read(3, t);
	a -= decayDiffusion2[0].read(1, t);
	a += postDampingDelay[0].read(1, t);
	return a;
}

float Dattorro::getRight(){
	float a = preDampingDelay[0].read(1, t);
	a += preDampingDelay[0].read(2, t);
	a -= decayDiffusion2[0].read(2, t);
	a += postDampingDelay[0].read(2, t);
	a -= preDampingDelay[1].read(3, t);
	a -= decayDiffusion2[1].read(1, t);
	a += postDampingDelay[1].read(1, t);
	return a;
}

// Main public callback====================================
float Dattorro::processMono(float in){
	processIn(in);
	return (getLeft() + getRight()) * 0.5f;
}
