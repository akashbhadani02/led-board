Pixel25 Studio — Mobile + PC browser control for ESP8266 and 25 WS2812B LEDs

FEATURES
- Responsive interface for desktop and mobile browser
- Upload image/video from computer or mobile gallery
- Live 5x5 pixel preview and Play/Pause/Stop
- Brightness and video speed controls
- Width/height selection (1 to 5)
- Custom LED map editor: tap/click grid cells to assign LED #1 through #25 in your actual physical order
- Manual pixel painting mode (select color and tap a grid cell)
- Default serpentine map

INSTALL
1) Arduino IDE: install Adafruit NeoPixel library.
2) Open esp8266/Pixel25_ESP8266.ino, enter Wi-Fi SSID/password, select ESP8266 board and upload.
3) On your computer install Node.js LTS from https://nodejs.org
4) Open a terminal in this folder and run: npm install
5) Run: npm start
6) PC: open http://localhost:3000
7) Phone: on the same Wi-Fi, open the LAN URL printed by the server, e.g. http://192.168.1.23:3000
8) Enter ESP IP address printed in Arduino Serial Monitor, then Connect/Save IP.

NOTES
- Node server and ESP must be on the same local Wi-Fi/LAN. If mobile cannot open the URL, allow Node.js through Windows Firewall and check that your router does not isolate wireless clients.
- WS2812B: DIN to ESP8266 D4 (GPIO2), 5V to external supply, and common GND. A 330-ohm series resistor on data is recommended. Use a suitably rated 5V supply.
- Video is converted in the browser to 5x5 RGB frames and streamed over WebSocket to Node, then UDP to ESP.
- This is a low-resolution 25-pixel display. Large/simple visuals work best.
