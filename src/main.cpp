
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include "SbcThread.h"
#include "OledMang.h"
#include "PowerCalculation.h"
#include "WiFi.h"
#include "BeeperCtrl.h"
// C0:49:EF:B4:3A:30
GetCtrlValue getV;
CalChannelValue calC;
WiFiUDP udp;
OledMang oled;
PowerCalculation powerCal(33);
BeeperCtrl beeper;

char incomingPacket[255]; // buffer for incoming packets
void setup()
{
  beeper.start();
  IPAddress _localIP, _gateway, _subnet, _dns;
  _localIP.fromString("192.168.0.177");
  _gateway.fromString("192.168.0.1");
  _subnet.fromString("255.255.255.0");
  _dns.fromString("8.8.8.8");
  Serial.begin(115200);
  // WiFi.config(_localIP, _gateway, _subnet, _dns);
  wl_status_t _state = WiFi.begin("TP-LINK_5EDC", "dahuang456");
  Serial.print("WiFi.begin: ");
  Serial.println(int(_state));
  while (true)
  {
    if (WiFi.isConnected())
    {
      Serial.println("connectted");
      udp.begin(5566);
      break;
    }
    else
    {
      delay(1000);
      Serial.println("connectting");
    }
  }

  BeeperCtrl::BeeperConfig task;
  task.playInternal = 100;
  task.stopInternal = 50;
  task.nums = 3;
  beeper.addBeeperTask(task);
}

void loop()
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
  GetCtrlValue::CtrlType _cType = getV.CtrlValue();
  if (_cType == GetCtrlValue::CtrlType::init)
  {
    calC.initCtrlValue(getV.ThrottleInitValue(), getV.HorizenDirectionInitValueL(), getV.VerticalDirectionInitValue(), getV.HorizenDirectionInitValueR());
  }
  else if (_cType == GetCtrlValue::CtrlType::open)
  {
    bool _fcr = calC.freshCtrlValue(getV.ThrottleValue(), getV.HorizenDirectionValueL(), getV.VerticalDirectionValue(), getV.HorizenDirectionValueR());
    if (WiFi.isConnected()&&_fcr)
    {
      //Serial.println("freshCtrlValue");
      JsonDocument _doc;
      _doc["channel1"] = calC.getChannelValue_1();
      _doc["channel2"] = calC.getChannelValue_2();
      _doc["channel3"] = calC.getChannelValue_3();
      _doc["channel4"] = calC.getChannelValue_4();

      String json;
      serializeJson(_doc, json);
      json = json + "*&*";
      udp.beginPacket("255.255.255.255", 5566);
      
      size_t _sendSize = udp.write((uint8_t *)json.c_str(), json.length());
      
      //Serial.println("发送变量");
      //Serial.print("calC.getChannelValue_1() ");
      //Serial.println(calC.getChannelValue_1());
      //Serial.print("calC.getChannelValue_2() ");
      //Serial.println(calC.getChannelValue_2());
      //Serial.print("calC.getChannelValue_3() ");
      //Serial.println(calC.getChannelValue_3());
      //Serial.print("calC.getChannelValue_4() ");
      //Serial.println(calC.getChannelValue_4());
      
      udp.endPacket();
    }
    //SbcStatus _status;
    //_status.m_masterVol = powerCal.getRemainPower();
    //_status.m_channel1 = calC.getChannelValue_1();
    //_status.m_channel2 = calC.getChannelValue_2();
    //_status.m_channel3 = calC.getChannelValue_3();
    //_status.m_channel4 = calC.getChannelValue_4();
    //oled.setSbcStatus(_status);
    //oled.run();
    //if (powerCal.getRemainPower() < 20);
    //{
    //  BeeperCtrl::BeeperConfig task;
    //  task.playInternal = 100;
    //  task.stopInternal = 100;
    //  task.nums = 1;
    //  beeper.addBeeperTask(task);
    //}
  }

  delay(2);
}
