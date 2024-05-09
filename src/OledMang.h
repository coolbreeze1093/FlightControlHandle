#pragma once
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include<string>
#include "common.h"
#include <mutex>
#include <sstream>

class OledMang
{
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
    SbcStatus m_sbcStatus;
    SbcStatus m_sbcStatusForShow;
    std::mutex m_mutex;
private:
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6);

public:
    OledMang(/* args */);
    ~OledMang();
    void run();
    void setSbcStatus(const SbcStatus&value);



};
