/*
 * Dattorro.cpp
 *
 *  Created on: Feb 16, 2025
 *      Author: hayden
 */
#include "Dattorro.h"
#include <algorithm>

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
}
