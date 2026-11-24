#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
const char* WIFI_SSID = "AB";
const char* WIFI_PASSWORD = "hiyan41725";
#define LED_PIN D4
#define LED_COUNT 25
#define UDP_PORT 4210
#define DISCOVERY_PORT 4211
#define BEACON_INTERVAL_MS 2500
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiUDP udp;
uint8_t brightness=166;
uint8_t packet[100];
unsigned long lastBeacon=0;
void sendDiscoveryBeacon(){
  if(WiFi.status()!=WL_CONNECTED)return;
  IPAddress local=WiFi.localIP(),mask=WiFi.subnetMask();
  IPAddress broadcast((uint8_t)(local[0]|(uint8_t)~mask[0]),(uint8_t)(local[1]|(uint8_t)~mask[1]),(uint8_t)(local[2]|(uint8_t)~mask[2]),(uint8_t)(local[3]|(uint8_t)~mask[3]));
  udp.beginPacket(broadcast,DISCOVERY_PORT);udp.write("PXDISC",6);udp.endPacket();lastBeacon=millis();
}
void setup(){
  Serial.begin(115200);strip.begin();strip.setBrightness(brightness);strip.clear();strip.show();
  WiFi.mode(WIFI_STA);WiFi.begin(WIFI_SSID,WIFI_PASSWORD);Serial.print("Joining Wi-Fi");
  while(WiFi.status()!=WL_CONNECTED){delay(350);Serial.print('.');}
  Serial.println();Serial.print("ESP IP: ");Serial.println(WiFi.localIP());udp.begin(UDP_PORT);Serial.printf("UDP listening: %u\n",UDP_PORT);sendDiscoveryBeacon();
}
void loop(){
  if(WiFi.status()!=WL_CONNECTED){WiFi.disconnect();WiFi.begin(WIFI_SSID,WIFI_PASSWORD);delay(1000);return;}
  if(millis()-lastBeacon>=BEACON_INTERVAL_MS)sendDiscoveryBeacon();
  int packetSize=udp.parsePacket();if(packetSize<=0)return;int n=udp.read(packet,sizeof(packet));
  if(n==5&&packet[0]=='P'&&packet[1]=='X'&&packet[2]=='L'&&packet[3]=='!'){brightness=packet[4];strip.setBrightness(brightness);strip.show();return;}
  if(n==79&&packet[0]=='P'&&packet[1]=='X'&&packet[2]=='2'&&packet[3]=='5'){for(int i=0;i<LED_COUNT;i++)strip.setPixelColor(i,strip.Color(packet[4+i*3],packet[5+i*3],packet[6+i*3]));strip.show();}
}
