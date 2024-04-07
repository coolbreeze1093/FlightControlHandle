
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include "OledMang.h"
#include "SbcThread.h"
#include "PowerCalculation.h"

// C0:49:EF:B4:3A:30
BeeperCtrl m_beeperCtrl;
GetCtrlValue getV;
SbcInterface *m_sbc = new sbc(&getV,&m_beeperCtrl);

OledMang m_oled;
SbcThread m_sbcthread(m_sbc);
PowerCalculation m_powerCal(33);
void setup()
{
  Serial.begin(115200);
  m_beeperCtrl.start();
  m_sbc->setChannel(0);
  m_sbc->setEncrypt(false);
  uint8_t peerAddr[] = {0xE4, 0x65, 0xB8, 0x48, 0xDE, 0x24};
  m_sbc->setMacAddr(peerAddr);

  if (EspNewMan_H.init())
  {
    if (EspNewMan_H.creatPeer(m_sbc))
    {
      Serial.println("success EspNewMan_H init");
    }
  }
  m_sbcthread.start(10);
}

void loop()
{
  m_oled.setMasterVolatile(std::to_string(m_powerCal.getRemainPower()));
  SbcInterface::ConnectState _state = m_sbc->getConnectState();
  std::string _oledConState;
  switch (_state)
  {
  case SbcInterface::ConnectState::connected:
    _oledConState = "已连接";
    break;
  case SbcInterface::ConnectState::disconnect:
    _oledConState = "连接中断";
    break;
  case SbcInterface::ConnectState::connectting:
    _oledConState = "正在连接";
    break;
  default:
    break;
  }
  m_oled.setChannelValue_1(std::to_string(getV.ThrottleValue()));
  m_oled.setChannelValue_2(std::to_string(getV.HorizenDirectionValueL()));
  m_oled.setChannelValue_3(std::to_string(getV.VerticalDirectionValue()));
  m_oled.setChannelValue_4(std::to_string(getV.HorizenDirectionValueR()));
  m_oled.setConnectState(_oledConState);
  m_oled.setSlaveVolatile(std::to_string(m_sbc->getClientVolatiol()));
  m_oled.run();
  delay(30);
}
