
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include "SbcThread.h"
#include "OledMang.h"
#include "PowerCalculation.h"

// C0:49:EF:B4:3A:30
BeeperCtrl m_beeperCtrl;
GetCtrlValue getV;
sbc m_sbc(&getV,&m_beeperCtrl);
OledMang m_oled;
SbcThread m_sbcthread(&m_sbc);
PowerCalculation m_powerCal(33);

void setup()
{
  Serial.begin(115200);
  m_beeperCtrl.start();
  m_sbc.setChannel(0);
  m_sbc.setEncrypt(false);
  uint8_t peerAddr[] = {0xE4, 0x65, 0xB8, 0x48, 0xDE, 0x24};
  m_sbc.setMacAddr(peerAddr);
  m_sbc.setOled(&m_oled);
  m_sbc.setPowerCalculation(&m_powerCal);
  if (EspNewMan_H.init())
  {
    if (EspNewMan_H.creatPeer(&m_sbc))
    {
      Serial.println("success EspNewMan_H init");
    }
  }
  m_sbcthread.start(2);
}

void loop()
{
  m_oled.run();
  delay(2);
}
