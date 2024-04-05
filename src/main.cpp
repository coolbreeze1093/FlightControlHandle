
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };



//C0:49:EF:B4:3A:30
SbcInterface *m_sbc = new sbc;
BeeperCtrl m_beeperCtrl;
void setup()
{
  Serial.begin(115200);
  m_beeperCtrl.start();
  m_sbc->setChannel(0);
  m_sbc->setEncrypt(false);
  uint8_t peerAddr[]={0xE4,0x65,0xB8,0x48,0xDE,0x24};
  m_sbc->setMacAddr(peerAddr);
  m_sbc->setBeeper(&m_beeperCtrl);

  if(EspNewMan_H.init())
  {
    if(EspNewMan_H.creatPeer(m_sbc))
    {
      Serial.println("success EspNewMan_H init");
    }
  }


  
  
}

void loop(){

  int16_t _volatile = analogReadMilliVolts(33);
  JsonDocument doc;
  Serial.println(_volatile);
  doc["volatile"]=_volatile;
  String json;
  serializeJson(doc, json);
  EspNewMan_H.send(m_sbc->getMacAddr(), (uint8_t *)json.c_str(), json.length());

  m_sbc->run();
  delay(10);
}
