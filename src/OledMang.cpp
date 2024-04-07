#include "OledMang.h"

/* data */
OledMang::OledMang(/* args */) : u8g2(U8G2_R0, 22, 21)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
}

OledMang::~OledMang()
{
}

void OledMang::run()
{
    u8g2.clearBuffer();                     // clear the internal memory
    u8g2.setFont(u8g2_font_wqy12_t_gb2312); // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do
    {
        u8g2.setCursor(0, 12);
        u8g2.print("连接状态:");
        u8g2.setCursor(70, 12);
        u8g2.print(m_connectState.c_str());
        u8g2.setCursor(0, 25);
        u8g2.print("遥控器电量:");
        u8g2.setCursor(70, 25);
        u8g2.print(m_masterVolatile.c_str());
        u8g2.setCursor(0, 38);
        u8g2.print("接收机电量:");
        u8g2.setCursor(70, 38);
        u8g2.print(m_slaveVolatile.c_str());
        u8g2.setCursor(0, 51);
        u8g2.print("通道1:");
        u8g2.setCursor(32, 51);
        u8g2.print(m_channelValue_1.c_str());
        u8g2.setCursor(64, 51);
        u8g2.print("通道2:");
        u8g2.setCursor(96, 51);
        u8g2.print(m_channelValue_2.c_str());
        u8g2.setCursor(0, 63);
        u8g2.print("通道3:");
        u8g2.setCursor(32, 63);
        u8g2.print(m_channelValue_3.c_str());
        u8g2.setCursor(64, 63);
        u8g2.print("通道4:");
        u8g2.setCursor(96, 63);
        u8g2.print(m_channelValue_4.c_str());
    } while (u8g2.nextPage());
}

void OledMang::setConnectState(const std::string &value)
{
    m_connectState = value;
}

void OledMang::setMasterVolatile(const std::string &value)
{
    m_masterVolatile = value;
}

void OledMang::setSlaveVolatile(const std::string &value)
{
    m_slaveVolatile = value;
}

void OledMang::setChannelValue_1(const std::string &value)
{
    m_channelValue_1=value;
}

void OledMang::setChannelValue_2(const std::string &value)
{
    m_channelValue_2=value;
}

void OledMang::setChannelValue_3(const std::string &value)
{
    m_channelValue_3=value;
}

void OledMang::setChannelValue_4(const std::string &value)
{
    m_channelValue_4=value;
}
