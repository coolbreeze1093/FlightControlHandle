
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include "OledMang.h"

//C0:49:EF:B4:3A:30
SbcInterface *m_sbc = new sbc;
BeeperCtrl m_beeperCtrl;
OledMang m_oled;
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

  m_oled.run();
  delay(10);
}
