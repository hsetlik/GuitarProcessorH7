#include "Freeverb.h"

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
