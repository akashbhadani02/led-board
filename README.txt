# Pixel25 Studio - Hotspot / No IP entry

ESP8266 creates a Wi-Fi hotspot and serves the player page. No Node bridge, LAN IP search, or IP field.

- Wi-Fi SSID: `Pixel25-LED`
- Password: `pixel25123`
- Web app: `http://192.168.4.1`

## Libraries
Install **Adafruit NeoPixel** and **WebSockets (Markus Sattler)**. ESP8266 core supplies ESP8266WebServer and LittleFS.

## Upload
1. Flash `esp8266/Pixel25_ESP8266.ino` from Arduino IDE.
2. Upload `data/index.html` to ESP8266 LittleFS using the ESP8266 LittleFS filesystem uploader tool.
3. Join the `Pixel25-LED` Wi-Fi from phone/PC (password `pixel25123`).
4. Browse to `http://192.168.4.1`, select media, and press Play.

Stop only pauses and leaves the latest frame displayed on the LEDs.

## Wiring
WS2812 DIN to D4/GPIO2, strip 5V from external regulated supply, grounds common, recommended 330 ohm series data resistor. Do not power the strip from ESP regulator.
