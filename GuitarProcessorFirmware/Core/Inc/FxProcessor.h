#ifndef FX_PROCESSOR_H
#define FX_PROCESSOR_H

#include "PedalState.h"
#include "main.h"

#define DC_BUF_LENGTH 2048
#ifdef __cplusplus
#include <cstdint>
#include <memory>
#include "FxAlgorithm.h"
#include "Dattorro.h"
#include "Schroeder.h"


class TransparentAlgorithm : public FxAlgorithm {
private:
public:
    TransparentAlgorithm(){

    }
    ~TransparentAlgorithm() override {

    }
    void processChunkMono(float* inBuf, float* outBuf, uint32_t numSamples) override;
    void processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples) override;
    void updateParams(pedal_state_t* ps) override;
};

class FxProcessor {
private:
    std::unique_ptr<FxAlgorithm> alg;
    uint8_t algIdx = 0;
    void prepareAlgorithm();
public:
    FxProcessor();
    void processChunk(float* in, float* out, uint32_t numSamples);
    void processChunkStereo(float* inL, float* inR, float* outL, float* outR, uint32_t numSamples);
    void updateParams(pedal_state_t* ps);
};


#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

typedef void* fx_processor_t;

EXTERNC fx_processor_t create_fx_processor();
// main processing callback
EXTERNC void process_fx_mono(fx_processor_t proc, float* input, float* output, uint32_t numSamples);
EXTERNC void process_fx_stereo(fx_processor_t proc, float* inL, float* inR, float* outL, float* outR, uint32_t numSamples);
EXTERNC void update_params(fx_processor_t proc, pedal_state_t* ps);
// DC measurement stuff
EXTERNC void init_dc_measurement_buf();
EXTERNC void push_dc_measurement_value(isample_t value);
EXTERNC isample_t get_dc_offset();

#undef EXTERNC


#endif