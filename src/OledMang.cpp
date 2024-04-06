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
    u8g2.clearBuffer();                         // clear the internal memory
    u8g2.setFont(u8g2_font_wqy12_t_gb2312); // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do
    {
        u8g2.setCursor(0, 13);
        u8g2.print("连接状态:");
        u8g2.setCursor(70, 13);
        u8g2.print("已连接");
        u8g2.setCursor(0, 26);
        u8g2.print("遥控器电量:"); 
        u8g2.setCursor(70, 26);
        u8g2.print("68%"); 
        u8g2.setCursor(0, 39);
        u8g2.print("接收机电量:"); 
        u8g2.setCursor(70, 39);
        u8g2.print("68%"); 
        u8g2.setCursor(0, 52);
        u8g2.print("通道1:"); 
        u8g2.setCursor(32, 52);
        u8g2.print("0"); 
        u8g2.setCursor(64, 52);
        u8g2.print("通道2:"); 
        u8g2.setCursor(96, 52);
        u8g2.print("0"); 
        u8g2.setCursor(0, 64);
        u8g2.print("通道2:"); 
        u8g2.setCursor(32, 64);
        u8g2.print("0"); 
        u8g2.setCursor(64, 64);
        u8g2.print("通道3:"); 
        u8g2.setCursor(96, 64);
        u8g2.print("0"); 
    } while (u8g2.nextPage());
    delay(1000); // transfer internal memory to the display
}
