#ifndef DTCM_POOL_H
#define DTCM_POOL_H

#ifdef __cplusplus
#include <cstddef>
namespace DTCMPool {
    float* alloc(size_t numFloats, bool zeroValues=false);
    void freePool();
    bool poolInUse();
}
#endif

#endif
