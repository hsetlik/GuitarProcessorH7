#include "DTCMPool.h"
#include "main.h"

// Small, CPU-only delay buffers (e.g. the Dattorro input/decay diffusion
// lines) live here so their accesses hit zero-wait-state DTCM instead of
// AXI SRAM. DTCMRAM is only 128K and is shared with .data/.bss/heap/stack,
// so keep this pool small and selective — see the budget note in the docs.
static constexpr size_t POOL_FLOATS = 4096;  // 16 KB

static float pool[POOL_FLOATS] __attribute__((section(".dtcm_pool")));
static size_t poolOffset = 0;

float* DTCMPool::alloc(size_t numFloats, bool zeroValues) {
    float* ptr = pool + poolOffset;
    if(poolOffset + numFloats >= POOL_FLOATS){
        Error_Handler();
    }
    poolOffset += numFloats;
    if(zeroValues){
        for(size_t i = 0; i < numFloats; ++i){
            ptr[i] = 0.0f;
        }
    }
    return ptr;
}

void DTCMPool::freePool(){
    poolOffset = 0;
}

bool DTCMPool::poolInUse(){
    return poolOffset > 0;
}
