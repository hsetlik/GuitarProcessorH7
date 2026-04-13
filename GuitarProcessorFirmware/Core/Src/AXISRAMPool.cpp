#include "AXISRAMPool.h"

static constexpr size_t POOL_FLOATS = 45000; // ~180 KB, fits all delay lines

__attribute__((section(".AXI_SRAM")))
static float pool[POOL_FLOATS];
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