#include "Freeverb.h"
#include "AXISRAMPool.h"
#include "main.h"

void LBCF::setBuffer(float* buffAdr, uint32_t size){
    buffer = buffAdr;
    bufSize = size;
    head = 0;
    for(uint32_t i = 0; i < bufSize; ++i){
        buffer[i] = 0.0f;
    }
}

void FVAllpass::setBuffer(float* buffAdr, uint32_t size){
    buffer = buffAdr;
    bufSize = size;
    head = 0;
    for(uint32_t i = 0; i < bufSize; ++i){
        buffer[i] = 0.0f;
    }
}
//=========================================================

namespace Tuning {
    // the numbers in the original freeverb implementation are optimized for a 44.1 kHz sample rate, so we 
    // use this factor to adjust to our working sample rate
    static const float tuneFactor = AUDIO_SAMPLE_RATE / 44100.0f;
    static const uint32_t stereoSpread = (uint32_t)(23.0f * tuneFactor);

    // comb lengths
    static const uint32_t combTuning[NUM_COMBS] = {
        (uint32_t)(1116.0f * tuneFactor),
        (uint32_t)(1188.0f * tuneFactor),
        (uint32_t)(1277.0f * tuneFactor),
        (uint32_t)(1356.0f * tuneFactor),
        (uint32_t)(1422.0f * tuneFactor),
        (uint32_t)(1491.0f * tuneFactor),
        (uint32_t)(1557.0f * tuneFactor),
        (uint32_t)(1617.0f * tuneFactor)
    };

    // allpass lengths
    static const uint32_t allpassTuning[NUM_ALLPASSES] = {
        (uint32_t)(556.0f * tuneFactor),
        (uint32_t)(441.0f * tuneFactor),
        (uint32_t)(341.0f * tuneFactor),
        (uint32_t)(225.0f * tuneFactor)
    };

    // default/fixed parameters
    static const float fixedGain = 0.15f;
    static const float scaleWet = 3.0f;
    static const float scaleDry = 2.0f;
    static const float scaleDamp = 0.4f;
    static const float scaleRoom = 0.28f;
    static const float offsetRoom = 0.7f;
    static const float initialRoom = 0.5f;
    static const float initialDamp = 0.5f;
    static const float initialWet = 1.0f / scaleWet;
    static const float initialDry = 0.0f;
    static const float initialWidth = 1.0f;
    static const float initialMode = 0.0f;
    static const float freezeMode = 0.5f;

    static const float roomsizeMin = 0.02f;
    static const float roomsizeMax = 1.0f;
    static const float roomsizeCenter = 0.35f;

    static const float dampMin = 0.1f;
    static const float dampMax = 0.85f;
    static const float dampCenter = 0.3f;

    static const float widthMin = 0.01f;
    static const float widthMax = 0.98f;
    static const float widthCenter = 0.25f;

    static const float wetMin = 0.02f;
    static const float wetMax = 1.0f;
    static const float wetCenter = 0.5f;
}

//=========================================================

void FreeverbAlg::setWet(float val){
    wet = val * Tuning::scaleWet;
}
void FreeverbAlg::setRoomsize(float val){
   roomsize = (val * Tuning::scaleRoom) + Tuning::offsetRoom; 
}
void FreeverbAlg::setDry(float val){
    dry = val * Tuning::scaleDry;
}
void FreeverbAlg::setDamp(float val){
    damp = val * Tuning::scaleDamp;
}
void FreeverbAlg::setWidth(float val){
    width = val;
}
void FreeverbAlg::setMode(float val){
    mode = val;
}

FreeverbAlg::FreeverbAlg(): 
roomRange(Tuning::roomsizeMin, Tuning::roomsizeMax, Tuning::roomsizeCenter),
dampRange(Tuning::dampMin, Tuning::dampMax, Tuning::dampCenter),
widthRange(Tuning::widthMin, Tuning::widthMax, Tuning::widthCenter),
wetRange(Tuning::wetMin, Tuning::wetMax, Tuning::wetCenter)
{
    // 1. initialize the filters
    for(uint32_t i = 0; i < NUM_COMBS; ++i){
        uint32_t lSize = Tuning::combTuning[i];
        uint32_t rSize = lSize + Tuning::stereoSpread;
        combsLeft[i].setBuffer(AXISRAMPool::alloc(lSize), lSize);
        combsRight[i].setBuffer(AXISRAMPool::alloc(rSize), rSize);
    }
    for(uint32_t i = 0; i < NUM_ALLPASSES; ++i){
        uint32_t lSize = Tuning::allpassTuning[i];
        uint32_t rSize = lSize + Tuning::stereoSpread;
        apLeft[i].setBuffer(AXISRAMPool::alloc(lSize), lSize);
        apLeft[i].setFeedback(0.5f);
        apRight[i].setBuffer(AXISRAMPool::alloc(rSize), rSize);
        apRight[i].setFeedback(0.5f);
    }
    // 2. set the parameters with their initial values
    setWet(Tuning::initialWet);
    setRoomsize(Tuning::initialRoom);
    setDry(Tuning::initialDry);
    setDamp(Tuning::initialDamp);
    setWidth(Tuning::initialWidth);
    setMode(Tuning::initialMode);
    // 3. update the other state variables as needed 
    updateState();
}

FreeverbAlg::~FreeverbAlg(){
    AXISRAMPool::freePool();
}

void FreeverbAlg::updateState(){
    wet1 = wet * (width*0.5f + 0.5f);
    wet2 = wet * ((1.0f - width) * 0.5f);
    if(mode >= Tuning::freezeMode){
        roomsize1 = 1.0f;
        damp1 = 0.0f;
        gain = 0.0f;
    } else {
        roomsize1 = roomsize;
        damp1 = damp;
        gain = Tuning::fixedGain;
    }
    for(uint32_t i = 0; i < NUM_COMBS; ++i){
        combsLeft[i].setFeedback(roomsize1);
        combsLeft[i].setDamping(damp1);
        combsRight[i].setFeedback(roomsize1);
        combsRight[i].setDamping(damp1);
    }
}


void FreeverbAlg::processSample(float input, float* outputL, float* outputR){
    float outR = 0.0f;
    float outL = 0.0f;
    float inVal = input * gain;
    // process combs (parallel)
    for(uint32_t i = 0; i < NUM_COMBS; ++i){
        outL += combsLeft[i].process(inVal);
        outR += combsRight[i].process(inVal);
    }
    // process allpasses (series)
    for(uint32_t i = 0; i < NUM_ALLPASSES; ++i){
        outL = apLeft[i].process(outL);
        outR = apRight[i].process(outR);
    }
    // mix wet/dry
    input *= dry;
    *outputL = (outL * wet1) + (outR * wet2) + input;
    *outputR = (outR * wet1) + (outL * wet2) + input;
}

void FreeverbAlg::processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples){
    float outL, outR;
    for(uint32_t i = 0; i < numSamples; ++i){
        processSample(inBuf[i], &outL, &outR);
        outBuf[i] = (outL + outR) / 2.0f;
    }
}

void FreeverbAlg::processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){
    for(uint32_t i = 0; i < numSamples; ++i){
        processSample(inL[i], &outL[i], &outR[i]);
    }
}

void FreeverbAlg::updateParams(pedal_state_t* ps){
    setRoomsize(roomRange.getDenormalized(ps->knobA));
    setDamp(dampRange.getDenormalized(1.0f - ps->knobB));
    setWidth(widthRange.getDenormalized(ps->knobC));
    const float wetVal = wetRange.getDenormalized(ps->knobD);
    setWet(wetVal);
    setDry(1.0f - wetVal);
    updateState();
}

