#ifndef NORMRANGE_H
#define NORMRANGE_H
#include "main.h"
#ifdef __cplusplus

// class for mapping knob values onto a skewed curve of float values
class NormRange {
    float minValue;
    float maxValue;
    float centerValue;
    float skew;
public:
    NormRange(float min=0.0f, float max=1.0f, float center=0.5f);
    void setRange(float min, float max, float center);
    float getDenormalized(float normValue);
};
#endif
#endif