#ifndef FREEVERB_H
#define FREEVERB_H

#ifdef __cplusplus
#include <cstdint>
#include "FxAlgorithm.h"
#include "main.h"
#include "NormRange.h"

// The lowpass feedback comb filter as described here: https://ccrma.stanford.edu/~jos/pasp/Lowpass_Feedback_Comb_Filter.html
// code pretty similar to the freeverb implementation on the CCRMA website

// Defines from the freeverb implementation
#define NUM_COMBS 8
#define NUM_ALLPASSES 4


//=========================================================================
class LBCF {
private:
    float* buffer = nullptr;
    uint32_t bufSize = 0;
    uint32_t head = 0;
    float feedback = 0.0f;
    float damp1 = 0.0f;
    float damp2 = 1.0f;
    float filterState = 0.0f;
public:
    LBCF(){}
    void setBuffer(float* buffAdr, uint32_t size);
    void setDamping(float val){
        damp1 = val;
        damp2 = 1.0f - val;
    }
    void setFeedback(float val){
        feedback = val;
    }
    inline float process(float input){
        float output = buffer[head];
        filterState = (output * damp2) + (filterState * damp1);
        buffer[head] = input + (filterState * feedback);
        head = (head + 1) % bufSize;
        return output;
    }
};

//==============================================================================

//Allpass approximation as described here: https://ccrma.stanford.edu/~jos/pasp/Freeverb_Allpass_Approximation.html
class FVAllpass {
private:
    float* buffer = nullptr;
    uint32_t bufSize = 0;
    uint32_t head = 0;
    float feedback = 0.5f;
public:
    FVAllpass(){}
    void setBuffer(float* startAdr, uint32_t size);
    void setFeedback(float fb){
        feedback = fb;
    }
    inline float process(float input){
        float bufHead = buffer[head];
        float output = -input + bufHead;
        buffer[head] = input + (bufHead * feedback);
        head = (head + 1) % bufSize;
        return output;
    }
};
//======================================================================

class FreeverbAlg : public FxAlgorithm {
private:
    // filter objects
    LBCF combsLeft[NUM_COMBS];
    LBCF combsRight[NUM_COMBS];
    FVAllpass apLeft[NUM_ALLPASSES];
    FVAllpass apRight[NUM_ALLPASSES];
    // state variables
    float gain;
	float roomsize, roomsize1;
	float damp, damp1;
	float wet, wet1, wet2;
	float dry;
	float width;
	float mode;
    // parameter setting handlers
    void setWet(float val);
    void setRoomsize(float val);
    void setDry(float val);
    void setDamp(float val);
    void setWidth(float val);
    void setMode(float val);


    void updateState();
    void processSample(float input, float* outputL, float* outputR);
    /* Controls are:
        Knob A: room size
        Knob B: damping
        Knob C: stereo width
        Knob D: wet/dry
    */
    NormRange roomRange;
    NormRange dampRange;
    NormRange widthRange;
    NormRange wetRange;
public:
    FreeverbAlg();
    ~FreeverbAlg();
    void processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples) override;
    void processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples) override;
    void updateParams(pedal_state_t* ps) override;
};

#endif
#endif