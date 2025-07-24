/*
 * RollingRMS.cpp
 *
 *  Created on: Jul 18, 2025
 *      Author: hayden
 */
#include "RollingRMS.h"

void RollingRMS::tick(float value) {
 // 1. subtract the oldest sample from the sums
	auto& oldest = ringBuf[head];
	meanSum -= oldest;
	rmsSum -= std::fabs(oldest);
// 2. add the new value to the sums
	meanSum += value;
	rmsSum += std::fabs(value);
// 3. push the new value into the ring buffer
	ringBuf[head] = value;
	head = (head + 1) % ROLLING_RMS_SIZE;
}

float RollingRMS::getRMSLevel() const {
	return rmsSum / (float)ROLLING_RMS_SIZE;
}

float RollingRMS::getDCBias() const {
	return meanSum / (float)ROLLING_RMS_SIZE;
}

