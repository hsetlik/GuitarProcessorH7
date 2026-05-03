#include "NormRange.h"


NormRange::NormRange(float min, float max, float center) : minValue(min), maxValue(max), centerValue(center){
    setRange(min, max, center);
}

void NormRange::setRange(float min, float max, float center){
    minValue = min;
    maxValue = max;
    centerValue = center;
    skew = std::logf(0.5f) / logf((centerValue -  minValue) / (maxValue - minValue));
}


float NormRange::getDenormalized(float normValue){
    const float proportion = std::expf(std::logf(normValue) / skew);
    return minValue + ((maxValue - minValue) * proportion);
}