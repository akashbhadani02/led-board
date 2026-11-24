#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";      // Set your Wi-Fi
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
#define LED_PIN D4
#define LED_COUNT 25
#define UDP_PORT 4210
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiUDP udp;
uint8_t brightness = 166; // ~65 percent
uint8_t packet[100];
void setup(){
  Serial.begin(115200);
  strip.begin(); strip.setBrightness(brightness); strip.clear(); strip.show();
  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Joining Wi-Fi");
  while(WiFi.status()!=WL_CONNECTED){delay(350);Serial.print('.');}
  Serial.println(); Serial.print("ESP IP: "); Serial.println(WiFi.localIP());
  udp.begin(UDP_PORT); Serial.printf("UDP listening: %u\n", UDP_PORT);
}
void loop(){
  int packetSize=udp.parsePacket(); if(packetSize<=0)return;
  int n=udp.read(packet,sizeof(packet));
  if(n==5 && packet[0]=='P'&&packet[1]=='X'&&packet[2]=='L'&&packet[3]=='!'){
    brightness=packet[4]; strip.setBrightness(brightness); strip.show(); return;
  }
  if(n==79 && packet[0]=='P'&&packet[1]=='X'&&packet[2]=='2'&&packet[3]=='5'){
    for(int i=0;i<LED_COUNT;i++) strip.setPixelColor(i,strip.Color(packet[4+i*3],packet[5+i*3],packet[6+i*3]));
    strip.show();
  }
}
