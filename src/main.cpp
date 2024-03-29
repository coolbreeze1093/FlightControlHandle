#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "CalChannelValue.h"
#include "GetCtrlValue.h"
#include "ArduinoJson.h"

const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 68);
const unsigned int remotePort = 25068;

WiFiUDP udp;
CalChannelValue calV;
GetCtrlValue getV;
bool OnlyDebugOnceInitValue=true;

void setup()
{
  Serial.begin(115200);
  
  IPAddress localIP;localIP.fromString("10.0.0.66");
  IPAddress gateway;gateway.fromString("10.0.0.1");
  IPAddress ipMask;ipMask.fromString("255.255.255.0");
  //IPAddress dhcp;dhcp.fromString("10.0.0.100");
  WiFi.softAPConfig(localIP,gateway,ipMask/*,dhcp*/);
  WiFi.softAP(ssid, password);

  udp.begin(25066);

  IPAddress _localIp = WiFi.softAPIP();
  String _ip = _localIp.toString();
  Serial.write(_ip.c_str());


}

void outInitValue()
{
  if(OnlyDebugOnceInitValue)
  {
    for(auto var:calV.getInitValue())
    {
      Serial.print(var.first.c_str());
      Serial.print("min:");
      Serial.print(var.second.min);
      Serial.print("init:");
      Serial.print(var.second.init);
      Serial.print("max:");
      Serial.print(var.second.max);
      Serial.println(" ");
    }
    OnlyDebugOnceInitValue = false;
  }
  
}

void loop(){
  // 检测是否有设备连接到当前的热点
  int connectedDevices = WiFi.softAPgetStationNum();
  if(connectedDevices <= 0) {
    Serial.print("有 ");
    Serial.print(connectedDevices);
    Serial.println(" 个设备连接到当前的热点");
    // 减少CPU的使用
    delay(1000);
    return;
  }
  
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }

    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
  JsonDocument doc;
  GetCtrlValue::CtrlType _cType = getV.CtrlValue();
  
  std::string _status="";
  switch (_cType)
  {
  case GetCtrlValue::CtrlType::close:
    outInitValue();
    _status="close";
    calV.quitInit();
    delay(1000);
    calV.initChannleValue();
    doc["status"] = _status;
    doc["channel1"] = calV.getChannelValue_1();
    doc["channel2"] = calV.getChannelValue_2();
    doc["channel3"] = calV.getChannelValue_3();
  break;
  case GetCtrlValue::CtrlType::open:
    {
      calV.freshCtrlValue(getV.ThrottleValue(),getV.HorizenDirectionValueL(),getV.VerticalDirectionValue(),getV.HorizenDirectionValueR());
      _status="open";
      doc["status"] = _status;
      doc["channel1"] = calV.getChannelValue_1();
      doc["channel2"] = calV.getChannelValue_2();
      doc["channel3"] = calV.getChannelValue_3();
    }
  break;
  case GetCtrlValue::CtrlType::init:
    {
      OnlyDebugOnceInitValue=true;
      calV.initCtrlValue(getV.ThrottleInitValue(),getV.HorizenDirectionInitValueL(),getV.VerticalDirectionInitValue(),getV.HorizenDirectionInitValueR());
      _status="init";
    }
  break;
  default:
    _status="error";
    break;
  }
  // Add an array
  //JsonArray data = doc["data"].to<JsonArray>();
  //data.add(48.756080);
  //data.add(2.302038);

  if(udp.beginPacket(remoteIp, remotePort)==0){
    delay(1000);
    return;
  }
  serializeJson(doc, udp);
  udp.println();
  if(udp.endPacket()==0){
      delay(1000);
      return;
  }
  
  delay(15);
}
