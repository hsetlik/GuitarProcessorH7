#ifndef AXI_SRAMPOOL_H
#define AXI_SRAMPOOL_H

#ifdef __cplusplus
#include <cstddef>
namespace AXISRAMPool {
    float* alloc(size_t numFloats);
    void freePool();
    bool poolInUse();
}
#endif

#endif