# Pixel25 Studio — Auto Wi-Fi Discovery

This project controls 25 WS2812B LEDs using ESP8266. The IP input/connect button has been removed. ESP broadcasts a `PXDISC` UDP beacon to port 4211; the PC's Node.js bridge detects it and sends frames to that discovered IP using UDP port 4210.

## Note about Vercel
Vercel may host a static UI, but live LED control requires the local Node.js UDP bridge on a PC on the same Wi-Fi/LAN as the ESP. For live control, use the local URL printed by `npm start`.

## Setup
1. Install Node.js LTS from https://nodejs.org.
2. Install the Adafruit NeoPixel library in Arduino IDE.
3. Open `esp8266/Pixel25_ESP8266.ino`, set Wi-Fi SSID/password (2.4 GHz), and upload to the ESP8266.
4. In the project folder run `npm install` once, then `npm start`.
5. On PC open `http://localhost:3000`. Wait for `Node: Connected` and `ESP: Connected`.
6. On mobile, open the LAN URL shown by the terminal; keep PC, mobile and ESP on the same Wi-Fi.
7. If Windows Firewall asks, allow Node.js on Private networks. UDP discovery port 4211 and data port 4210 should be allowed on your local network.

## Features
- Photo/video playback, 5x5 preview
- Running brand text ticker (up to 200 chars)
- Brightness, video speed, contrast, color strength, dithering, auto-brighten
- Manual painting and customizable LED mapping
- Auto WebSocket reconnect
- Saves UI controls, LED map and painted pixels in browser local storage

## Wiring
WS2812B DIN -> ESP D4/GPIO2; LED +5V -> external 5V; LED GND and ESP GND common. Use a 330-470 ohm data resistor and a suitable 5V power supply.
