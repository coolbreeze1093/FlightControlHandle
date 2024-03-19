#ifndef CALCTRLVALUE
#define CALCTRLVALUE
#include <Arduino.h>
// 定义模拟输入引脚
const uint8_t ThrottleButton = 39; 
const uint8_t HorizenDirectionButtonL = 36; 
const uint8_t VerticalDirectionButton = 34; 
const uint8_t HorizenDirectionButtonR = 35; 
class CalCtrlValue
{
public:
    CalCtrlValue();
    ~CalCtrlValue();

    uint8_t *value();

    

private: 
    
    int16_t lastThrottleValue = 0;
    int16_t lastHorizenDirectionValueL = 0;
    int16_t lastVerticalDirectionValue = 0;
    int16_t lastHorizenDirectionValueR = 0;
    int16_t lastCtrlValue = 0;
}


#endif
