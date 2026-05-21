#include "FxProcessor.h"
#include "Dattorro.h"
#include "FxAlgorithm.h"
#include "Schroeder.h"
#include "Freeverb.h"

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
    //flushDenormalsToZero(true);
}

void FxProcessor::prepareAlgorithm(){
    if(algIdx == 1){
        alg.reset(new SchroederAlg());
    } else if (algIdx == 2){
        alg.reset(new FreeverbAlg());
    } else if (algIdx == 3){
        alg.reset(new Dattorro1Alg());
    } else {
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