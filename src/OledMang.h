#pragma once
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include<string>


class OledMang
{
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
    std::string m_connectState;
    std::string m_masterVolatile;
    std::string m_slaveVolatile;
    std::string m_channelValue_1;
    std::string m_channelValue_2;
    std::string m_channelValue_3;
    std::string m_channelValue_4;

public:
    OledMang(/* args */);
    ~OledMang();
    void run();
    void setConnectState(const std::string&value);
    void setMasterVolatile(const std::string&value);
    void setSlaveVolatile(const std::string&value);
    void setChannelValue_1(const std::string&value);
    void setChannelValue_2(const std::string&value);
    void setChannelValue_3(const std::string&value);
    void setChannelValue_4(const std::string&value);

};
