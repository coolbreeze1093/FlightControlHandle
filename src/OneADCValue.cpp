#include "OneADCValue.h"
#include "Arduino.h"

OneADCValue::OneADCValue(int pin,int filterSize,int threshold):m_pin(pin),m_filterSize(filterSize),m_threshold(threshold),m_averageFilter(filterSize)
{
    
}

OneADCValue::~OneADCValue()
{
}

float OneADCValue::update()
{
  int16_t _value = analogRead(m_pin);
  float _result = m_averageFilter.update(_value);
  if(m_lastValue==_result)
  {
    m_stableTimes++;
  }
  else{
    m_stableTimes=0;
  }
  if(abs(_result-m_lastValue)>=m_threshold||m_stableTimes>50)
  {
    m_finalValue = _result;
  }
  m_lastValue = _result;
  return m_finalValue;
}