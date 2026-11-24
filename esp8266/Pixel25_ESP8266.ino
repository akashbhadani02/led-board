#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>
#include <LittleFS.h>
#define LED_PIN D4
#define LED_COUNT 25
const char* AP_SSID="Pixel25-LED";
const char* AP_PASS="pixel25123";
const uint8_t INITIAL_BRIGHTNESS=166;
Adafruit_NeoPixel strip(LED_COUNT,LED_PIN,NEO_GRB+NEO_KHZ800);
ESP8266WebServer http(80);
WebSocketsServer ws(81);
uint8_t frame[75];
void applyFrame(){for(int i=0;i<25;i++)strip.setPixelColor(i,strip.Color(frame[i*3],frame[i*3+1],frame[i*3+2]));strip.show();}
void onWs(uint8_t client,WStype_t type,uint8_t *payload,size_t len){
 if(type==WStype_BIN&&len==79&&payload[0]=='P'&&payload[1]=='X'&&payload[2]=='2'&&payload[3]=='5'){memcpy(frame,payload+4,75);applyFrame();}
 else if(type==WStype_TEXT){String msg((char*)payload);int k=msg.indexOf("value");if(msg.indexOf("brightness")>=0&&k>=0){int colon=msg.indexOf(':',k);int v=constrain(msg.substring(colon+1).toInt(),0,255);strip.setBrightness(v);applyFrame();}}
}
void setup(){Serial.begin(115200);strip.begin();strip.setBrightness(INITIAL_BRIGHTNESS);strip.clear();strip.show();memset(frame,0,sizeof(frame));WiFi.mode(WIFI_AP);WiFi.softAPConfig(IPAddress(192,168,4,1),IPAddress(192,168,4,1),IPAddress(255,255,255,0));WiFi.softAP(AP_SSID,AP_PASS);Serial.println(WiFi.softAPIP());if(!LittleFS.begin())Serial.println("LittleFS mount error");http.on("/",HTTP_GET,[]{if(LittleFS.exists("/index.html"))http.streamFile(LittleFS,"/index.html","text/html");else http.send(500,"text/plain","Upload index.html to LittleFS.");});http.onNotFound([]{http.send(404,"text/plain","Not found");});http.begin();ws.begin();ws.onEvent(onWs);}
void loop(){http.handleClient();ws.loop();}
