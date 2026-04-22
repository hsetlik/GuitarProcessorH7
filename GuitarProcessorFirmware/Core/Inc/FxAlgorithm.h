#ifndef FX_ALGORITHM_H
#define FX_ALGORITHM_H
#include "main.h"
#include <cstdint>

#ifdef __cplusplus
#include "PedalState.h"

// the parent class for any and all effects
class FxAlgorithm {
public:
    FxAlgorithm()=default;
    virtual ~FxAlgorithm();
    virtual void processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples)=0;
    virtual void processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){}
    virtual void updateParams(pedal_state_t* ps)=0;
};

#endif
#endif