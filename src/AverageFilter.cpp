#include "AverageFilter.h"
#include <numeric>
AverageFilter::AverageFilter(int size):m_filterSize(size)
{
}

AverageFilter::~AverageFilter()
{
}

float AverageFilter::update(float value)
{
    if(m_valueVec.size()>=m_filterSize)
    {
        m_valueVec.pop_back();
    }
    m_valueVec.insert(m_valueVec.begin(),value/1000);

    float sum = std::accumulate(m_valueVec.begin(), m_valueVec.end(), 0.0);
    
    sum /= m_valueVec.size();

    return sum*1000;
}