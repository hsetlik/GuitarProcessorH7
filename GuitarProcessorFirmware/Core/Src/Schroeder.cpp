#include "Schroeder.h"
#include <cstdint>
#include "AXISRAMPool.h"
#define WET_MIN 0.2f
#define WET_MAX 0.95f
#define FB_MIN 0.5f
#define FB_MAX 0.95f
#define DAMP_MIN 0.1f
#define DAMP_MAX 0.85f


static const uint32_t COMB_LENGTHS[]   = {1116, 1188, 1277, 1356};
static const uint32_t STEREO_WIDTH = 22;
static const uint32_t ALLPASS_LENGTHS[] = {556, 441};



SchroederAlg::SchroederAlg(){
    for(uint16_t i = 0; i < NUM_COMB_FILTERS; ++i){
        auto& lComb = combsLeft[i];
        lComb.buffer = AXISRAMPool::alloc(COMB_LENGTHS[i]);
        lComb.length = COMB_LENGTHS[i];
        for(uint32_t i = 0; i < lComb.length; ++i){
            lComb.buffer[i] = 0.0f;
        }
        lComb.head = 0;
        lComb.lpState = 0.0f;
        auto& rComb = combsRight[i];
        rComb.buffer = AXISRAMPool::alloc(COMB_LENGTHS[i] + STEREO_WIDTH);
        rComb.length = COMB_LENGTHS[i] + STEREO_WIDTH;
        for(uint32_t i = 0; i < rComb.length; ++i){
            rComb.buffer[i] = 0.0f;
        }
        rComb.head = 0;
        rComb.lpState = 0.0f;
    }
    for(uint16_t i = 0; i < NUM_ALLPASS; ++i){
        auto& apL = apLeft[i];
        apL.buffer = AXISRAMPool::alloc(ALLPASS_LENGTHS[i]);
        apL.length = ALLPASS_LENGTHS[i];
        for(uint32_t i = 0; i < apL.length; ++i){
            apL.buffer[i] = 0.0f;
        }
        apL.feedback = 0.5f;
        apL.head = 0;
        auto& apR = apRight[i];
        apR.buffer = AXISRAMPool::alloc(ALLPASS_LENGTHS[i]);
        apR.length = ALLPASS_LENGTHS[i];
        for(uint32_t i = 0; i < apR.length; ++i){
            apR.buffer[i] = 0.0f;
        }
        apR.feedback = 0.5f;
        apR.head = 0;
    }
}

SchroederAlg::~SchroederAlg(){
    AXISRAMPool::freePool();
}

float SchroederAlg::processSampleMono(float input){
    const float dry = input;
    input *= 0.8f;
    float out = 0.0f;

    // comb filters
    for(uint16_t i = 0; i < NUM_COMB_FILTERS; ++i){
        auto& comb = combsLeft[i];
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
        auto& ap = apLeft[i];
        float delayed = ap.buffer[ap.head];
        ap.buffer[ap.head] = out + (delayed * ap.feedback);
        ap.head = (ap.head + 1) % ap.length;
        out = delayed - (out * ap.feedback);
    }
    return (dry * (1.0f - wetLvl)) + (out * wetLvl);
}


void SchroederAlg::processSampleStereo(float input, float* oL, float* oR){
    const float dry = input;
    input *= 0.2f;
    float outL = 0.0f;
    float outR = 0.0f;

    // comb filters
    for(uint16_t i = 0; i < NUM_COMB_FILTERS; ++i){
        auto& combL = combsLeft[i];
        // grab the delayed sample
        float delayedL = combL.buffer[combL.head];
        // process the lowpass filter w damping
        combL.lpState = (delayedL * nDamp) + (combL.lpState * damping);
        // apply the decay value
        float feedbackSampleL = combL.lpState * feedback;
        // update the delay line
        combL.buffer[combL.head] = input + feedbackSampleL;
        combL.head = (combL.head + 1) % combL.length;
        // add the delayed to the output
        outL += delayedL;

        // same thing for right combs
        auto& combR = combsRight[i];
        float delayedR = combR.buffer[combR.head];
        combR.lpState = (delayedR * nDamp) + (combR.lpState * damping);
        float feedbackSampleR = combR.lpState * feedback;
        combR.buffer[combR.head] = input + feedbackSampleR;
        combR.head = (combR.head + 1) % combR.length;
        outR += delayedR;
    }

    // allpass filters
    for(uint16_t i = 0; i < NUM_ALLPASS; ++i){
        auto& apL = apLeft[i];
        float delayedL = apL.buffer[apL.head];
        apL.buffer[apL.head] = outL + (delayedL * apL.feedback);
        apL.head = (apL.head + 1) % apL.length;
        outL = delayedL - (outL * apL.feedback);
        auto& apR = apRight[i];
        float delayedR = apR.buffer[apR.head];
        apR.buffer[apR.head] = outR + (delayedR * apR.feedback);
        apR.head = (apR.head + 1) % apR.length;
        outR = delayedR - (outR * apR.feedback);
    }
    *oL = (dry * nWet) + (outL * wetLvl);
    *oR = (dry * nWet) + (outR * wetLvl);
}

void SchroederAlg::processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples){
    for(uint32_t i = 0; i < numSamples; ++i){
        outBuf[i] = processSampleMono(inBuf[i]);
    }
}

void SchroederAlg::processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){
    for(uint32_t i = 0; i < numSamples; ++i){
        processSampleStereo(inL[i], &outL[i], &outR[i]);
    }
}

void SchroederAlg::updateParams(pedal_state_t* ps){
   feedback = flerp(FB_MIN, FB_MAX, ps->knobA); 
   damping = flerp(DAMP_MIN, DAMP_MAX, ps->knobB); 
   nDamp = 1.0f - damping;
   wetLvl = flerp(WET_MIN, WET_MAX, ps->knobC); 
   nWet = 1.0f - wetLvl;
}