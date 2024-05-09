#include <Arduino.h>
#include "GetCtrlValue.h"
#include "esp_wifi.h"
#include "driver/adc.h"

GetCtrlValue::GetCtrlValue():m_Throttle(ThrottleButton,FilterSize,Threshold)
,m_HorizenDirectionValueL(HorizenDirectionButtonL,FilterSize,Threshold)
,m_VerticalDirectionValue(VerticalDirectionButton,FilterSize,Threshold)
,m_HorizenDirectionValueR(HorizenDirectionButtonR,FilterSize,Threshold)
{}

GetCtrlValue::~GetCtrlValue()
{
}
int16_t GetCtrlValue::ThrottleValue()
{
  return m_Throttle.update();
}
int16_t GetCtrlValue::HorizenDirectionValueL()
{
  return m_HorizenDirectionValueL.update();
}
int16_t GetCtrlValue::VerticalDirectionValue()
{
  return m_VerticalDirectionValue.update();
}
int16_t GetCtrlValue::HorizenDirectionValueR()
{
  return m_HorizenDirectionValueR.update();
}

int16_t GetCtrlValue::ThrottleInitValue()
{
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  return _ThrottleValue;
}

int16_t GetCtrlValue::HorizenDirectionInitValueL()
{
  int16_t _HorizenDirectionLValue = analogRead(HorizenDirectionButtonL);
  return _HorizenDirectionLValue;
}

int16_t GetCtrlValue::VerticalDirectionInitValue()
{
  int16_t _VerticalDirectionValue = analogRead(VerticalDirectionButton);
  return _VerticalDirectionValue;
}

int16_t GetCtrlValue::HorizenDirectionInitValueR()
{
  int16_t _HorizenDirectionRValue = analogRead(HorizenDirectionButtonR);
  return _HorizenDirectionRValue;
}
GetCtrlValue::CtrlType GetCtrlValue::CtrlValue()
{
  int16_t _CtrlValue = analogRead(32);
  //Serial.println(_CtrlValue);
  if (_CtrlValue > 3800)
  {
    return CtrlType::init;
  }
  else if (_CtrlValue < 2000 && _CtrlValue > 1300)
  {
    return CtrlType::open;
  }
  else if (_CtrlValue > 800 && _CtrlValue < 1300)
  {
    return CtrlType::close;
  }
  else
  {
    return CtrlType::error;
  }
}