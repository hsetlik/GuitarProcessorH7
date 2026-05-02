#ifndef FREEVERB_H
#define FREEVERB_H

#include "main.h"
#ifdef __cplusplus

// The lowpass feedback comb filter as described here: https://ccrma.stanford.edu/~jos/pasp/Lowpass_Feedback_Comb_Filter.html
// code pretty similar to the freeverb implementation on the CCRMA website

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

#endif
#endif