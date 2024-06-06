
#include <Arduino.h>
#include "EspNewMan.h"
#include "sbc.h"
#include "SbcThread.h"
#include "OledMang.h"
#include "PowerCalculation.h"
#include "WiFi.h"
// C0:49:EF:B4:3A:30
GetCtrlValue getV;
CalChannelValue calC;
WiFiUDP udp;
char incomingPacket[255];  // buffer for incoming packets
void setup()
{
  IPAddress _localIP, _gateway, _subnet, _dns;
  _localIP.fromString("192.168.0.123");
  _gateway.fromString("192.168.0.1");
  _subnet.fromString("255.255.255.0");
  _dns.fromString("192.168.1.1");
  Serial.begin(115200);
  WiFi.config(_localIP, _gateway, _subnet, _dns);
  wl_status_t _state = WiFi.begin("TP-LINK_5EDC", "dahuang456");
  Serial.print("WiFi.begin: ");
  Serial.println(int(_state));
  while (true)
  {
    if (WiFi.isConnected())
    {
      udp.begin(5566);
      break;
    }
    else{
      delay(1000);
    }
  }
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
    calC.freshCtrlValue(getV.ThrottleValue(), getV.HorizenDirectionValueL(), getV.VerticalDirectionValue(), getV.HorizenDirectionValueR());
    JsonDocument _doc;
    _doc["channel1"]=calC.getChannelValue_1();
    _doc["channel2"]=calC.getChannelValue_2();
    _doc["channel3"]=calC.getChannelValue_3();
    _doc["channel4"]=calC.getChannelValue_4();

    if (WiFi.isConnected())
    {
      String json;
      serializeJson(_doc, json);
      udp.beginPacket("192.168.0.105",5566);
      udp.write((uint8_t *)json.c_str(), json.length());
      udp.endPacket();
    }
  }

  delay(10);
}
