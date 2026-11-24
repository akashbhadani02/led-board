#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "webui.h"

#define LED_PIN D4
#define LED_COUNT 25

const char* AP_NAME = "Pixel25-LED";
const char* AP_PASS = "hiyan41725";

ESP8266WebServer server(80);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint8_t currentFrame[LED_COUNT * 3] = {0};
uint8_t ledMap[LED_COUNT];

void applyFrame(const uint8_t* data) {
  for (int i = 0; i < LED_COUNT; i++) {
    int physical = ledMap[i];
    uint8_t r = data[i * 3];
    uint8_t g = data[i * 3 + 1];
    uint8_t b = data[i * 3 + 2];
    strip.setPixelColor(physical, strip.Color(r, g, b));
  }
  strip.show();
}


void setup() {
  Serial.begin(115200);
  delay(100);

  strip.begin();
  strip.setBrightness(110);
  strip.clear();
  strip.show();

  EEPROM.begin(32);

  bool mapOk = (EEPROM.read(0) == 0xA5);
  bool seen[25] = {false};

  if (mapOk) {
    for (int i = 0; i < LED_COUNT; i++) {
      uint8_t v = EEPROM.read(i + 1);
      if (v >= 25 || seen[v]) {
        mapOk = false;
        break;
      }
      seen[v] = true;
      ledMap[i] = v;
    }
  }

  // Default serpentine mapping if no valid mapping is stored.
  if (!mapOk) {
    for (int i = 0; i < LED_COUNT; i++) {
      int row = i / 5;
      int col = i % 5;
      if (row % 2 == 1) col = 4 - col;
      ledMap[i] = row * 5 + col;
    }
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NAME, AP_PASS);

  Serial.println();
  Serial.println("Pixel25 WiFi AP started");
  Serial.print("WiFi Name: ");
  Serial.println(AP_NAME);
  Serial.print("Open on phone: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", PIXEL25_PAGE);
  });

  server.on("/frame", HTTP_POST, []() {
    String s = server.arg("plain");
    if (s.length() == 75) {
      for (int i = 0; i < 75; i++) currentFrame[i] = (uint8_t)s[i];
      applyFrame(currentFrame);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", String("Expected 75 RGB bytes; received ") + s.length());
    }
  });

  server.on("/mapping", HTTP_POST, []() {
    String s = server.arg("plain");
    if (s.length() != 25) {
      server.send(400, "text/plain", "Expected 25 mapping bytes");
      return;
    }

    bool used[25] = {false};
    bool valid = true;
    for (int i = 0; i < 25; i++) {
      uint8_t v = (uint8_t)s[i];
      if (v >= 25 || used[v]) {
        valid = false;
        break;
      }
      used[v] = true;
    }

    if (!valid) {
      server.send(400, "text/plain", "Mapping must contain unique values 0-24");
      return;
    }

    for (int i = 0; i < 25; i++) {
      ledMap[i] = (uint8_t)s[i];
      EEPROM.write(i + 1, ledMap[i]);
    }
    EEPROM.write(0, 0xA5);
    EEPROM.commit();

    applyFrame(currentFrame);
    server.send(200, "text/plain", "Mapping saved permanently");
  });

  server.on("/status", HTTP_GET, []() {
    server.send(200, "application/json", "{\"ok\":true,\"leds\":25,\"mode\":\"AP\"}");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
  server.handleClient();
}
