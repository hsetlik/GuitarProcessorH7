#include "FxProcessor.h"
#include "AXISRAMPool.h"
#include "Dattorro.h"
#include "FxAlgorithm.h"
#include "Schroeder.h"
#include "Freeverb.h"
#include "main.h"


void TransparentAlgorithm::processChunkMono(float* inBuf, float* outBuf, uint32_t length){
    for(uint32_t i = 0; i < length; ++i){
        outBuf[i] = inBuf[i];
    }
}


void TransparentAlgorithm::processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){
    for(uint32_t i = 0; i < numSamples; ++i){
        outL[i] = inL[i];
        outR[i] = inR[i];
    }
}

void TransparentAlgorithm::updateParams(pedal_state_t*){

}

// helper for Denormal handling--------------------------------
void flushDenormalsToZero(bool shouldFlush){
    uint32_t prevValue = __get_FPSCR();
    uint32_t newValue;
    if(shouldFlush){
       newValue = prevValue | (1 << 24); 
    } else {
        newValue = prevValue & ~(1 << 24);
    }
    __set_FPSCR(newValue);
}

//==========================================================

FxProcessor::FxProcessor() : alg(std::make_unique<TransparentAlgorithm>()), algIdx(0){
    flushDenormalsToZero(true);
}

void FxProcessor::prepareAlgorithm(){
    if(algIdx == 1){
        alg.reset(new SchroederAlg());
    } else if (algIdx == 2){
        alg.reset(new FreeverbAlg());
    } 
    // else if (algIdx == 3){
    //     alg.reset(new Dattorro1Alg());
    // } 
    else {
        alg.reset(new TransparentAlgorithm());
    }
}

void FxProcessor::processChunk(float* in, float* out, uint32_t numSamples){
    alg->processChunkMono(in, out, numSamples);
}


void FxProcessor::processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){
    //flushDenormalsToZero(true);
    alg->processChunkStereo(inL, inR, outL, outR, numSamples);
    //flushDenormalsToZero(false);
}

void FxProcessor::updateParams(pedal_state_t* ps){
    if(ps->algIdx != algIdx){
        algIdx = ps->algIdx;
        prepareAlgorithm();
    }
    alg->updateParams(ps);
}


//==========================================================

fx_processor_t create_fx_processor(){
	return new FxProcessor();
}

void process_fx_mono(fx_processor_t proc, float* input, float* output, uint32_t numSamples){
    FxProcessor* ptr = static_cast<FxProcessor*>(proc);
    ptr->processChunk(input, output, numSamples);
}

void process_fx_stereo(fx_processor_t proc, float* inL, float* inR, float* outL, float* outR, uint32_t numSamples){
    FxProcessor* ptr = static_cast<FxProcessor*>(proc);
    ptr->processChunkStereo(inL, inR, outL, outR, numSamples);
}

void update_params(fx_processor_t proc, pedal_state_t* ps){
    FxProcessor* ptr = static_cast<FxProcessor*>(proc);
    ptr->updateParams(ps);
}

isample_t dcBuf[DC_BUF_LENGTH]__attribute__((section(".axisram_pool")));
static const float dcDivisor = (float)DC_BUF_LENGTH;
static uint32_t head = 0;
static uint32_t dcSum = 0;
void init_dc_measurement_buf(){
    for(uint32_t i = 0; i < DC_BUF_LENGTH; ++i){
        dcBuf[i] = 0;
    }
}
void push_dc_measurement_value(isample_t value){
    // 1. add the new value to the sum
    dcSum += value;
    // 2. subtract the oldest value from the sum
    dcSum -= dcBuf[head];
    // 3. overwrite the oldest value
    dcBuf[head] = value;
    // 4. increment the head
    head = (head + 1) % DC_BUF_LENGTH;
}
isample_t get_dc_offset(){
    return (isample_t)((float)dcSum / dcDivisor);
}