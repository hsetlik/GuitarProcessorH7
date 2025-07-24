/*
 * RollingRMS.h
 *
 *  Created on: Jul 18, 2025
 *      Author: hayden
 */

#ifndef INC_ROLLINGRMS_H_
#define INC_ROLLINGRMS_H_
#define ROLLING_RMS_SIZE 128

#ifdef __cplusplus
#include "stm32h7xx_hal.h"

class RollingRMS {
private:
	float ringBuf[ROLLING_RMS_SIZE];
	uint16_t head = 0;
	float meanSum;
	float rmsSum;
public:
	RollingRMS()=default;
	void tick(float value);
	float getRMSLevel() const;
	float getDCBias() const;
};


#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_ROLLINGRMS_H_ */
