#ifndef SCHROEDER_H
#define SCHROEDER_H
#ifdef __cplusplus
#include "AXISRAMPool.h"
#include "FxAlgorithm.h"

#define NUM_COMBS 4
#define NUM_ALLPASS 2

struct CombFilter {
    float* buffer;
    uint32_t length;
    uint32_t head;
    float lpState;
};

struct AllpassFilter {
    float* buffer;
    uint32_t length;
    uint32_t head;
    float feedback;
};

class SchroederAlg : public FxAlgorithm {
private:
    CombFilter combsLeft[NUM_COMBS];
    CombFilter combsRight[NUM_COMBS];
    AllpassFilter apLeft[NUM_ALLPASS];
    AllpassFilter apRight[NUM_ALLPASS];
    float damping = 0.5f;
    float nDamp = 0.5f;
    float feedback = 0.6f;
    float wetLvl = 0.3f;
    float nWet = 0.7f;
    float processSampleMono(float input);
    void processSampleStereo(float input, float* outL, float* outR);

public:
    SchroederAlg();
    ~SchroederAlg() override;
    void processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples) override;
    void processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples) override;
    void updateParams(pedal_state_t* ps) override;
};




#endif
#endif