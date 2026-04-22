#include "Schroeder.h"
#include <cstdint>
#include "AXISRAMPool.h"
#define WET_MIN 0.2f
#define WET_MAX 0.95f
#define FB_MIN 0.5f
#define FB_MAX 0.95f
#define DAMP_MIN 0.1f
#define DAMP_MAX 0.85f
static inline float flerp(float a, float b, float prop){
    return a + ((b - a) * prop);
}

static const uint32_t COMB_LENGTHS[]   = {1116, 1188, 1277, 1356};
static const uint32_t ALLPASS_LENGTHS[] = {556, 441};



SchroederAlg::SchroederAlg(){
    for(uint16_t i = 0; i < NUM_COMBS; ++i){
        auto& comb = combs[i];
        comb.buffer = AXISRAMPool::alloc(COMB_LENGTHS[i]);
        comb.length = COMB_LENGTHS[i];
        for(uint32_t i = 0; i < comb.length; ++i){
            comb.buffer[i] = 0.0f;
        }
        comb.head = 0;
        comb.lpState = 0.0f;
    }
    for(uint16_t i = 0; i < NUM_ALLPASS; ++i){
        auto& ap = aps[i];
        ap.buffer = AXISRAMPool::alloc(ALLPASS_LENGTHS[i]);
        ap.length = ALLPASS_LENGTHS[i];
        for(uint32_t i = 0; i < ap.length; ++i){
            ap.buffer[i] = 0.0f;
        }
        ap.feedback = 0.5f;
        ap.head = 0;
    }
}

SchroederAlg::~SchroederAlg(){
    AXISRAMPool::freePool();
}

float SchroederAlg::processSample(float input){
    const float dry = input;
    input *= 0.2f;
    float out = 0.0f;

    // comb filters
    for(uint16_t i = 0; i < NUM_COMBS; ++i){
        auto& comb = combs[i];
        // grab the delayed sample
        float delayed = comb.buffer[comb.head];
        // process the lowpass filter w damping
        comb.lpState = (delayed * (1.0f - damping)) + (comb.lpState * damping);
        // apply the decay value
        float feedbackSample = comb.lpState * feedback;
        // update the delay line
        comb.buffer[comb.head] = input + feedbackSample;
        comb.head = (comb.head + 1) % comb.length;
        // add the delayed to the output
        out += delayed;
    }

    // allpass filters
    for(uint16_t i = 0; i < NUM_ALLPASS; ++i){
        auto& ap = aps[i];
        float delayed = ap.buffer[ap.head];
        ap.buffer[ap.head] = out + (delayed * ap.feedback);
        ap.head = (ap.head + 1) % ap.length;
        out = delayed - (out * ap.feedback);
    }
    return (dry * (1.0f - wetLvl)) + (out * wetLvl);
}

void SchroederAlg::processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples){
    for(uint32_t i = 0; i < numSamples; ++i){
        outBuf[i] = processSample(inBuf[i]);
    }
}

void SchroederAlg::updateParams(pedal_state_t* ps){
   feedback = flerp(FB_MIN, FB_MAX, ps->knobA); 
   damping = flerp(DAMP_MIN, DAMP_MAX, ps->knobB); 
   wetLvl = flerp(WET_MIN, WET_MAX, ps->knobC); 
}