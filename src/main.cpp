#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 100);
const unsigned int remotePort = 25068;

// 定义模拟输入引脚
const uint8_t ThrottleButton = 39; 
const uint8_t HorizenDirectionButtonL = 36; 
const uint8_t VerticalDirectionButton = 34; 
const uint8_t HorizenDirectionButtonR = 35; 

int16_t lastThrottleValue = 0;
int16_t lastHorizenDirectionValueL = 0;
int16_t lastVerticalDirectionValue = 0;
int16_t lastHorizenDirectionValueR = 0;
int16_t lastCtrlValue = 0;

const uint8_t ThrottleButtonSign = 0xA0;
const uint8_t HorizenDirectionButtonLSign = 0xA1;
const uint8_t VerticalDirectionButtonSign = 0xA2;
const uint8_t HorizenDirectionButtonRSign = 0xA3;

uint8_t SendData[6]={0xAA,0x00,0x00,0x00,0xFF,0xFF};

bool onlyRevToSendThrottle = true;
bool onlyRevToSendHorizenDirectionL = true;
bool onlyRevToSendVerticalDirection = true;
bool onlyRevToSendHorizenDirectionR = true;

WiFiUDP udp;

IPAddress setlocalIp()
{
 IPAddress localIP;localIP.fromString("10.0.0.66");
return localIP;
}

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress localIP;localIP.fromString("10.0.0.66");
  IPAddress gateway;gateway.fromString("10.0.0.1");
  IPAddress ipMask;ipMask.fromString("255.255.255.0");
  IPAddress dhcp;dhcp.fromString("10.0.0.100");
  WiFi.softAPConfig(localIP,gateway,ipMask,dhcp);

  udp.begin(25066);

  IPAddress _localIp = WiFi.softAPIP();
  String _ip = _localIp.toString();
  Serial.write(_ip.c_str());
}

bool ThrottleButtonFunc(){
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  if(abs(_ThrottleValue-lastThrottleValue)>20||
  (_ThrottleValue==0&&_ThrottleValue!=lastThrottleValue)||
  (_ThrottleValue==0x0FFF&&_ThrottleValue!=lastThrottleValue))
  {
    // 开始一个UDP包
    if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendData[1] = 0xA0;
    SendData[2] = (_ThrottleValue >> 8) & 0xFF; // 右移8位得到高位
    SendData[3] = _ThrottleValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendData,sizeof(SendData));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      lastThrottleValue = _ThrottleValue;
      return true;
    }}
  else{}
  return false;
}
bool HorizenDirectionButtonLFunc(){
int16_t _HorizenDirectionLValue = analogRead(HorizenDirectionButtonL);
  if(abs(_HorizenDirectionLValue-lastHorizenDirectionValueL)>20||
  (_HorizenDirectionLValue==0&&_HorizenDirectionLValue!=lastHorizenDirectionValueL)||
  (_HorizenDirectionLValue==0x0FFF&&_HorizenDirectionLValue!=lastHorizenDirectionValueL))
  {
    // 开始一个UDP包
    if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendData[1] = 0xA1;
    SendData[2] = (_HorizenDirectionLValue >> 8) & 0xFF; // 右移8位得到高位
    SendData[3] = _HorizenDirectionLValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendData,sizeof(SendData));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      lastHorizenDirectionValueL = _HorizenDirectionLValue;
      return true;
    }}
  else{}
  return false;
}
bool VerticalDirectionButtonFunc(){
int16_t _VerticalDirectionValue = analogRead(VerticalDirectionButton);
  if(abs(_VerticalDirectionValue-lastVerticalDirectionValue)>20||
  (_VerticalDirectionValue==0&&_VerticalDirectionValue!=lastVerticalDirectionValue)||
  (_VerticalDirectionValue==0x0FFF&&_VerticalDirectionValue!=lastVerticalDirectionValue))
  {
    // 开始一个UDP包
    if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendData[1] = 0xA2;
    SendData[2] = (_VerticalDirectionValue >> 8) & 0xFF; // 右移8位得到高位
    SendData[3] = _VerticalDirectionValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendData,sizeof(SendData));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      lastVerticalDirectionValue = _VerticalDirectionValue;
      return true;
    }}
  else{
  }
  return false;
}
bool HorizenDirectionButtonRFunc(){
  int16_t _HorizenDirectionRValue = analogRead(HorizenDirectionButtonR);
  if(abs(_HorizenDirectionRValue-lastHorizenDirectionValueR)>20||
  (_HorizenDirectionRValue==0&&_HorizenDirectionRValue!=lastHorizenDirectionValueR)||
  (_HorizenDirectionRValue==0x0FFF&&_HorizenDirectionRValue!=lastHorizenDirectionValueR))
  {
    // 开始一个UDP包
    if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendData[1] = 0xA3;
    SendData[2] = (_HorizenDirectionRValue >> 8) & 0xFF; // 右移8位得到高位
    SendData[3] = _HorizenDirectionRValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendData,sizeof(SendData));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      lastHorizenDirectionValueR = _HorizenDirectionRValue;
      return true;
    }}
  else{}
  return false;
};

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

  if(onlyRevToSendThrottle)
  {
    if(ThrottleButtonFunc())
    {
      //onlyRevToSendThrottle=false;
    }
  }
  else{
    Serial.println("油门数据发送失败");
  }
  if(onlyRevToSendHorizenDirectionL)
  {
    if(HorizenDirectionButtonLFunc())
    {
      //onlyRevToSendHorizenDirectionL=false;
    }
  }else{
    Serial.println("左方向数据发送失败");
  }
  if(onlyRevToSendVerticalDirection)
  {
    if(VerticalDirectionButtonFunc())
    {
      //onlyRevToSendVerticalDirection=false;
    }
  }
  else{
    Serial.println("右垂直方向数据发送失败");
  }
  if(onlyRevToSendHorizenDirectionR)
  {
    if(HorizenDirectionButtonRFunc())
    {
      //onlyRevToSendHorizenDirectionR=false;
    }
  }
  else{
    Serial.println("右水平方向数据发送失败");
  }

  int16_t _CtrlValue = analogRead(32);
  if()
  if(_CtrlValue<450)
  {
    Serial.println(_CtrlValue);
  }
  else if(_CtrlValue>3800){
    Serial.println(_CtrlValue);
  }
  else if(_CtrlValue<3800&&_CtrlValue>2300)
  {
    Serial.println(_CtrlValue);
  }

  lastCtrlValue = _CtrlValue;
  
  // 减少CPU的使用
  delay(10);
}
