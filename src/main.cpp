#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP32_Udp_wzs";
const char* password = "ssdwifi66";

const IPAddress remoteIp(10, 0, 0, 100);
const unsigned int remotePort = 25068;

// 定义模拟输入引脚
const uint8_t ThrottleButton = 39; 
const uint8_t HorizenDirectionButton = 36; 
const uint8_t DirectionButton2 = 34; 
const uint8_t DirectionButton3 = 35; 

int16_t ThrottleValue = 0;

bool onlyRevToSend = true;

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

  char buffer[255];
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }

    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }

  // 开始一个UDP包
  udp.beginPacket(remoteIp, remotePort);

  // 读取模拟输入A0
  int16_t analogValue = analogRead(ThrottleButton);

  int16_t high = (analogValue >> 8) & 0xFF; // 右移8位得到高位
  int16_t low = analogValue & 0xFF; // 与0xFF进行位与运算得到低位

  // 将模拟输入的值转换为字符串
  //sprintf(buffer,"%d %d %d %d %d %d",0xA1,0xA2,high,low,0xFF,0xFF);
  //udp.println(buffer);

  int16_t _buffer[6]={0xA1,0xA2,high,low,0xFF,0xFF};

  

  // 结束并发送UDP包
  if(udp.endPacket()==0)
  {
    delay(1000);
  }

  // 减少CPU的使用
  delay(10);
}
