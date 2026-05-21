#include "AXISRAMPool.h"

static constexpr size_t POOL_FLOATS = 55000;

static float pool[POOL_FLOATS] __attribute__((section(".axisram_pool")));
static size_t poolOffset = 0;

float* AXISRAMPool::alloc(size_t numFloats) {
    float* ptr = pool + poolOffset;
    poolOffset += numFloats;
    return ptr;
}

void AXISRAMPool::freePool(){
    poolOffset = 0;
}

bool AXISRAMPool::poolInUse(){
    return poolOffset > 0;
}