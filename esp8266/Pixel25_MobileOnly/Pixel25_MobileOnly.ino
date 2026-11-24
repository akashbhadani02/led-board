#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "webui.h"

#define LED_PIN D4
#define LED_COUNT 25
#define UDP_UNUSED 0
const char* AP_NAME = "Pixel25-LED";
const char* AP_PASS = "pixel2588"; // 8+ chars
ESP8266WebServer server(80);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint8_t currentFrame[LED_COUNT*3];
uint8_t ledMap[LED_COUNT];

void applyFrame(const uint8_t* data){
  // Physical serpentine layout: row 0 L->R, row 1 R->L, etc.
  for(int y=0;y<5;y++) for(int x=0;x<5;x++){
    int logical=y*5+x;
    int physical=ledMap[logical];
    uint8_t r=data[logical*3], g=data[logical*3+1], b=data[logical*3+2];
    strip.setPixelColor(physical, strip.Color(r,g,b));
  }
  strip.show();
}
void setup(){
  Serial.begin(115200); delay(100);
  strip.begin(); strip.setBrightness(110); strip.clear(); strip.show();
  EEPROM.begin(32);
  bool mapOk=EEPROM.read(0)==0xA5; bool seen[25]={false};
  if(mapOk){ for(int i=0;i<25;i++){ uint8_t v=EEPROM.read(i+1); if(v>=25||seen[v]) mapOk=false; else seen[v]=true; ledMap[i]=v; } }
  if(!mapOk) for(int i=0;i<LED_COUNT;i++) ledMap[i]=(i/5)*5+((i/5)%2?(4-(i%5)):(i%5));
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NAME, AP_PASS);
  Serial.print("Pixel25 WiFi AP: "); Serial.println(AP_NAME);
  Serial.print("Open in phone: http://"); Serial.println(WiFi.softAPIP());
  server.on("/", HTTP_GET, [](){ server.send_P(200,"text/html",PIXEL25_PAGE); });
  server.on("/frame", HTTP_POST, [](){
    String s=server.arg("plain");
    if(s.length()==75){ for(int i=0;i<75;i++) currentFrame[i]=(uint8_t)s[i]; applyFrame(currentFrame); server.send(200,"text/plain","OK"); }
    else server.send(400,"text/plain",String("Expected 75 RGB bytes; received ")+s.length());
  });
  server.on("/mapping", HTTP_POST, [](){
    String s=server.arg("plain");
    if(s.length()!=25){ server.send(400,"text/plain","Expected 25 mapping bytes"); return; }
    bool used[25]={false},valid=true;
    for(int i=0;i<25;i++){ uint8_t v=(uint8_t)s[i]; if(v>=25||used[v])valid=false; else used[v]=true; }
    if(!valid){ server.send(400,"text/plain","Mapping must contain unique values 0-24"); return; }
    for(int i=0;i<25;i++){ledMap[i]=(uint8_t)s[i]; EEPROM.write(i+1,ledMap[i]);} EEPROM.write(0,0xA5); EEPROM.commit();
    applyFrame(currentFrame); server.send(200,"text/plain","Mapping saved permanently");
  });
  server.on("/status", HTTP_GET, [](){ server.send(200,"application/json","{\"ok\":true,\"leds\":25,\"mode\":\"AP\"}"); });
  server.onNotFound([](){server.send(404,"text/plain","Not found");});
  server.begin();
}
void loop(){ server.handleClient(); }
