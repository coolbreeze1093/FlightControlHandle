#pragma once
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

class OledMang
{
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
public:
    OledMang(/* args */);
    ~OledMang();
    void run();
};
