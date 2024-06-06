#include "OledMang.h"
#include <iomanip>
/* data */
OledMang::OledMang(/* args */) : u8g2(U8G2_R0, 22, 21)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312); // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFontDirection(0);
}

OledMang::~OledMang()
{
}

void OledMang::run()
{
    {
        std::lock_guard<std::mutex> _lock(m_mutex);
        m_sbcStatusForShow = m_sbcStatus;
    }

    u8g2.firstPage();
    do
    {
        u8g2.setCursor(0, 12);
        u8g2.print("遥控器电量:");
        u8g2.setCursor(70, 12);
        u8g2.print(std::to_string(m_sbcStatusForShow.m_masterVol).c_str());
        u8g2.setCursor(0, 25);
        u8g2.print("接收机电量:");
        u8g2.setCursor(70, 25);
        u8g2.print(std::to_string(m_sbcStatusForShow.m_slaveVol).c_str());
        u8g2.setCursor(0, 38);
        u8g2.print("通道1:");
        u8g2.setCursor(35, 38);
        u8g2.print(to_string_with_precision<float>(m_sbcStatusForShow.m_channel1,2).c_str());
        u8g2.setCursor(64, 38);
        u8g2.print("通道2:");
        u8g2.setCursor(99, 38);
        u8g2.print(to_string_with_precision<float>(m_sbcStatusForShow.m_channel2,1).c_str());
        u8g2.setCursor(0, 51);
        u8g2.print("通道3:");
        u8g2.setCursor(35, 51);
        u8g2.print(to_string_with_precision<float>(m_sbcStatusForShow.m_channel3,1).c_str());
        u8g2.setCursor(64, 51);
        u8g2.print("通道4:");
        u8g2.setCursor(99, 51);
        u8g2.print(to_string_with_precision<float>(m_sbcStatusForShow.m_channel4,1).c_str());
    } while (u8g2.nextPage());
}

void OledMang::setSbcStatus(const SbcStatus &value)
{
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_sbcStatus = value;
}
template <typename T>
std::string OledMang::to_string_with_precision(const T a_value, const int n)
{

    int nn = n;
    std::ostringstream out;
    out << std::fixed << std::setprecision(nn) << a_value;
    return out.str();
}
