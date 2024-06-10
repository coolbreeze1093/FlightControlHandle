#ifndef CALCTRLVALUE
#define CALCTRLVALUE
#include "OneADCValue.h"

class GetCtrlValue
{
public:
enum class CtrlType
{
    error=-1,
    init=0,
    open=1,
    close=2
};
public:
    GetCtrlValue();
    ~GetCtrlValue();

    int16_t ThrottleValue();
    int16_t HorizenDirectionValueL();
    int16_t VerticalDirectionValue();
    int16_t HorizenDirectionValueR();

    int16_t ThrottleInitValue();
    int16_t HorizenDirectionInitValueL();
    int16_t VerticalDirectionInitValue();
    int16_t HorizenDirectionInitValueR();
    CtrlType CtrlValue();

private: 
    // 定义模拟输入引脚
    const uint8_t ThrottleButton = 39; 
    const uint8_t HorizenDirectionButtonL = 36; 
    const uint8_t VerticalDirectionButton = 34; 
    const uint8_t HorizenDirectionButtonR = 35; 
    const uint8_t FilterSize=10;
    const uint8_t Threshold=0;

    OneADCValue m_Throttle;
    OneADCValue m_HorizenDirectionValueL;
    OneADCValue m_VerticalDirectionValue;
    OneADCValue m_HorizenDirectionValueR;
};


#endif
