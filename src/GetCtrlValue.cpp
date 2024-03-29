#include <Arduino.h>
#include "GetCtrlValue.h"

GetCtrlValue::GetCtrlValue()
{

}
GetCtrlValue::~GetCtrlValue()
{

}
int16_t GetCtrlValue::ThrottleValue(){
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  if(abs(_ThrottleValue-m_lastThrottleValue)>20||
  (_ThrottleValue==0&&_ThrottleValue!=m_lastThrottleValue)||
  (_ThrottleValue==0x0FFF&&_ThrottleValue!=m_lastThrottleValue))
  {
    m_lastThrottleValue = _ThrottleValue;
  }
  
  return m_lastThrottleValue;
  
}
int16_t GetCtrlValue::HorizenDirectionValueL(){
    int16_t _HorizenDirectionLValue = analogRead(HorizenDirectionButtonL);
  if(abs(_HorizenDirectionLValue-m_lastHorizenDirectionValueL)>20||
  (_HorizenDirectionLValue==0&&_HorizenDirectionLValue!=m_lastHorizenDirectionValueL)||
  (_HorizenDirectionLValue==0x0FFF&&_HorizenDirectionLValue!=m_lastHorizenDirectionValueL))
  {
    m_lastHorizenDirectionValueL = _HorizenDirectionLValue;
  }
  return m_lastHorizenDirectionValueL;
}
int16_t GetCtrlValue::VerticalDirectionValue(){
    int16_t _VerticalDirectionValue = analogRead(VerticalDirectionButton);
  if(abs(_VerticalDirectionValue-m_lastVerticalDirectionValue)>20||
  (_VerticalDirectionValue==0&&_VerticalDirectionValue!=m_lastVerticalDirectionValue)||
  (_VerticalDirectionValue==0x0FFF&&_VerticalDirectionValue!=m_lastVerticalDirectionValue))
  {
    m_lastVerticalDirectionValue = _VerticalDirectionValue;
  }
  return m_lastVerticalDirectionValue;
}
int16_t GetCtrlValue::HorizenDirectionValueR(){
    int16_t _HorizenDirectionRValue = analogRead(HorizenDirectionButtonR);
  if(abs(_HorizenDirectionRValue-m_lastHorizenDirectionValueR)>20||
  (_HorizenDirectionRValue==0&&_HorizenDirectionRValue!=m_lastHorizenDirectionValueR)||
  (_HorizenDirectionRValue==0x0FFF&&_HorizenDirectionRValue!=m_lastHorizenDirectionValueR))
  {
    m_lastHorizenDirectionValueR = _HorizenDirectionRValue;
  }
  return m_lastHorizenDirectionValueR;
}

int16_t GetCtrlValue::ThrottleInitValue()
{
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  m_lastThrottleValue = _ThrottleValue;
  return m_lastThrottleValue;
}

int16_t GetCtrlValue::HorizenDirectionInitValueL()
{
  int16_t _HorizenDirectionLValue = analogRead(HorizenDirectionButtonL);
  m_lastHorizenDirectionValueL = _HorizenDirectionLValue;
  return m_lastHorizenDirectionValueL;
}

int16_t GetCtrlValue::VerticalDirectionInitValue()
{
  int16_t _VerticalDirectionValue = analogRead(VerticalDirectionButton);
  m_lastVerticalDirectionValue = _VerticalDirectionValue;
  return m_lastVerticalDirectionValue;
}

int16_t GetCtrlValue::HorizenDirectionInitValueR()
{
  int16_t _HorizenDirectionRValue = analogRead(HorizenDirectionButtonR);
  m_lastHorizenDirectionValueR = _HorizenDirectionRValue;
  return m_lastHorizenDirectionValueR;
}
GetCtrlValue::CtrlType GetCtrlValue::CtrlValue(){
    int16_t _CtrlValue = analogRead(32);
  if(_CtrlValue>3800){
    return CtrlType::init;
  }
  else if(_CtrlValue<3800&&_CtrlValue>2300)
  {
    return CtrlType::open;
  }
  else if(_CtrlValue>800)
  {
    return CtrlType::error;
  }
  else{
    return CtrlType::close;
  }
}