#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 68);
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

/*const uint8_t ThrottleButtonSign = 0xA0;
const uint8_t HorizenDirectionButtonLSign = 0xA1;
const uint8_t VerticalDirectionButtonSign = 0xA2;
const uint8_t HorizenDirectionButtonRSign = 0xA3;*/

uint8_t SendDataforCtrl[7]={0xAA,0xB0,0x00,0x00,0x00,0x00,0xFF};
uint8_t SendDataforInit[7]={0xAA,0xB1,0x00,0x00,0x00,0x00,0xFF};
uint8_t SendDataforOpen[7]={0xAA,0xB2,0xA0,0xFF,0xFF,0xFF,0xFF};


bool onlyRevToSendThrottle = true;
bool onlyRevToSendHorizenDirectionL = true;
bool onlyRevToSendVerticalDirection = true;
bool onlyRevToSendHorizenDirectionR = true;

bool initOnlyOnce=false;
bool openOnlyOnce=false;
bool closeOnlyOnce=true;
bool isClosed=false;

WiFiUDP udp;

IPAddress setlocalIp()
{
 IPAddress localIP;localIP.fromString("10.0.0.66");
return localIP;
}

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

bool ThrottleButtonFunc(){
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  if(abs(_ThrottleValue-lastThrottleValue)>20||
  (_ThrottleValue==0&&_ThrottleValue!=lastThrottleValue)||
  (_ThrottleValue==0x0FFF&&_ThrottleValue!=lastThrottleValue))
  {
    SendDataforCtrl[3] = (_ThrottleValue >> 8) & 0xFF; // 右移8位得到高位
    SendDataforCtrl[4] = _ThrottleValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendDataforCtrl,sizeof(SendDataforCtrl));
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
    SendDataforCtrl[2] = 0xA1;
    SendDataforCtrl[3] = (_HorizenDirectionLValue >> 8) & 0xFF; // 右移8位得到高位
    SendDataforCtrl[4] = _HorizenDirectionLValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendDataforCtrl,sizeof(SendDataforCtrl));
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
    SendDataforCtrl[2] = 0xA2;
    SendDataforCtrl[3] = (_VerticalDirectionValue >> 8) & 0xFF; // 右移8位得到高位
    SendDataforCtrl[4] = _VerticalDirectionValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendDataforCtrl,sizeof(SendDataforCtrl));
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
    SendDataforCtrl[2] = 0xA3;
    SendDataforCtrl[3] = (_HorizenDirectionRValue >> 8) & 0xFF; // 右移8位得到高位
    SendDataforCtrl[4] = _HorizenDirectionRValue & 0xFF; // 与0xFF进行位与运算得到低位
    udp.write(SendDataforCtrl,sizeof(SendDataforCtrl));
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

bool initCtrlValue()
{
  int16_t _ThrottleValue = analogRead(ThrottleButton);
  if(udp.beginPacket(remoteIp, remotePort)==0){
    return false;
  }
  SendDataforInit[2] = 0xA0;
  SendDataforInit[3] = (_ThrottleValue >> 8) & 0xFF; // 右移8位得到高位
  SendDataforInit[4] = _ThrottleValue & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(SendDataforInit,sizeof(SendDataforInit));
  if(udp.endPacket()==0){
    delay(1000);
    return false;
  }
  int16_t _HorizenDirectionLValue = analogRead(HorizenDirectionButtonL);
  if(udp.beginPacket(remoteIp, remotePort)==0){
    return false;
  }
  SendDataforInit[2] = 0xA1;
  SendDataforInit[3] = (_HorizenDirectionLValue >> 8) & 0xFF; // 右移8位得到高位
  SendDataforInit[4] = _HorizenDirectionLValue & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(SendDataforInit,sizeof(SendDataforInit));
  if(udp.endPacket()==0){
    delay(1000);
    return false;
  }
  int16_t _VerticalDirectionValue = analogRead(VerticalDirectionButton);
  // 开始一个UDP包
  if(udp.beginPacket(remoteIp, remotePort)==0){
    return false;
  }
  SendDataforInit[2] = 0xA2;
  SendDataforInit[3] = (_VerticalDirectionValue >> 8) & 0xFF; // 右移8位得到高位
  SendDataforInit[4] = _VerticalDirectionValue & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(SendDataforInit,sizeof(SendDataforInit));
  // 结束并发送UDP包
  if(udp.endPacket()==0){
    delay(1000);
    return false;
  }
  int16_t _HorizenDirectionRValue = analogRead(HorizenDirectionButtonR);
  // 开始一个UDP包
  if(udp.beginPacket(remoteIp, remotePort)==0){
    return false;
  }
  SendDataforInit[2] = 0xA3;
  SendDataforInit[3] = (_HorizenDirectionRValue >> 8) & 0xFF; // 右移8位得到高位
  SendDataforInit[4] = _HorizenDirectionRValue & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(SendDataforInit,sizeof(SendDataforInit));
  // 结束并发送UDP包
  if(udp.endPacket()==0){
    delay(1000);
    return false;
  }
    
  return true;
}

bool openCtrlValue()
{
  if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendDataforOpen[4]=0x01;
    udp.write(SendDataforOpen,sizeof(SendDataforOpen));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      return true;
    }
    return false;
}

bool closeCtrlValue()
{
  if(udp.beginPacket(remoteIp, remotePort)==0){
      return false;
    }
    SendDataforOpen[4]=0x00;
    udp.write(SendDataforOpen,sizeof(SendDataforOpen));
    // 结束并发送UDP包
    if(udp.endPacket()==0){
      delay(1000);
      return false;
    }
    else{
      return true;
    }
    return false;
}
void test()
{
  // 开始一个UDP包
  if(udp.beginPacket(remoteIp, remotePort)==0){
    return;
  }

  uint8_t _buffer[3];
  int16_t _v = analogReadMilliVolts(ThrottleButton);
  Serial.println("Throttle value:");
  Serial.print(_v);
  Serial.println(" ");
  _buffer[0] = 0xA0;
  _buffer[1] = (_v >> 8) & 0xFF; // 右移8位得到高位
  _buffer[2] = _v & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(_buffer,sizeof(_buffer));

  _v = analogReadMilliVolts(HorizenDirectionButtonL);
  Serial.println("HorizenDirectionButtonL value:");
  Serial.print(_v);
  Serial.println(" ");
  _buffer[0] = 0xA1;
  _buffer[1] = (_v >> 8) & 0xFF; // 右移8位得到高位
  _buffer[2] = _v & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(_buffer,sizeof(_buffer));

   _v = analogReadMilliVolts(VerticalDirectionButton);
  Serial.println("VerticalDirectionButton value:");
  Serial.print(_v);
  Serial.println(" ");
  _buffer[0] = 0xA1;
  _buffer[1] = (_v >> 8) & 0xFF; // 右移8位得到高位
  _buffer[2] = _v & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(_buffer,sizeof(_buffer));

   _v = analogReadMilliVolts(HorizenDirectionButtonR);
  Serial.println("HorizenDirectionButtonR value:");
  Serial.print(_v);
  Serial.println(" ");
  _buffer[0] = 0xA1;
  _buffer[1] = (_v >> 8) & 0xFF; // 右移8位得到高位
  _buffer[2] = _v & 0xFF; // 与0xFF进行位与运算得到低位
  udp.write(_buffer,sizeof(_buffer));

  if(udp.endPacket()==0){
      delay(1000);
      return;
  }

  delay(500);
  return;
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
  if(_CtrlValue>3800&&initOnlyOnce==true){
    if(initCtrlValue())
    {
      initOnlyOnce=false;
    }
    isClosed=false;
    Serial.println("init");
  }
  else if(_CtrlValue>3800&&initOnlyOnce==false){
    isClosed=false;
  }
  else if(_CtrlValue<3800&&_CtrlValue>2300&&openOnlyOnce==true)
  {
    if(openCtrlValue())
    {
      openOnlyOnce=false;
    }
    closeOnlyOnce=true;
    isClosed=false;
    Serial.println("open");
  }
  else if(_CtrlValue<3800&&_CtrlValue>2300&&openOnlyOnce==false){
    isClosed=false;
  }
  else if(_CtrlValue>800)
  {
    Serial.println("error");
  }
  else{
    isClosed=true;
    
  }

  if(isClosed&&closeOnlyOnce==true){
    if(closeCtrlValue())
    {
      closeOnlyOnce=false;
    }
  }

  if(isClosed==true)
  {
    initOnlyOnce=true;
    openOnlyOnce=true;
  }

  lastCtrlValue = _CtrlValue;
  
  // 减少CPU的使用
  delay(40);
}
