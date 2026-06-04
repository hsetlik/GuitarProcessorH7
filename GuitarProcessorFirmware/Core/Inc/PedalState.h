#ifndef PEDAL_STATE_H
#define PEDAL_STATE_H
#include "main.h"

typedef struct {
    uint8_t algIdx;
    bool fxEngaged;
    float knobA;
    float knobB;
    float knobC;
    float knobD;
    bool expButton;
} pedal_state_t;

// initializes the pedal state's values
void PedalState_init(pedal_state_t* ps);

// returns the data for the LEDs based on the state
uint8_t PedalState_getLedData(pedal_state_t* ps);

#endif