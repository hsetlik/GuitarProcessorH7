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
    CombFilter combs[NUM_COMBS];
    AllpassFilter aps[NUM_ALLPASS];
    float damping = 0.5f;
    float feedback = 0.6f;
    float wetLvl = 0.3f;
    float processSample(float input);

public:
    SchroederAlg();
    ~SchroederAlg() override;
    void processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples) override;
    void updateParams(pedal_state_t* ps) override;
};




#endif
#endif