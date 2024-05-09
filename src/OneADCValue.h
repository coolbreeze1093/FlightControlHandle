#pragma once
#include "AverageFilter.h"
class OneADCValue
{
private:
    /* data */
    int m_pin=-1;
    int m_filterSize=0;
    float m_lastValue=0;
    float m_finalValue=0;
    float m_threshold=0;
    AverageFilter m_averageFilter;
    int m_stableTimes=0;
public:
    OneADCValue(int pin,int filterSize,int threshold);
    ~OneADCValue();

    float update();


};


