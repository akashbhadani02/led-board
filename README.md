# Pixel25 Mobile-Only (ESP8266 AP mode)

This version does **not** need a PC, Node.js, Vercel, a router, or manual IP entry. ESP8266 creates its own Wi-Fi hotspot and hosts the control page.

## Wi-Fi and page
- Wi-Fi SSID: `Pixel25-LED`
- Password: `pixel2588`
- Open on your phone: `http://192.168.4.1`

When your phone asks whether to stay connected to Wi-Fi without internet, choose **Stay connected / Keep connected**. Mobile data can remain enabled, but some Android phones may switch away from a Wi-Fi network without internet; turn off automatic network switching if needed.

## Arduino IDE setup
1. Install ESP8266 board support in Arduino IDE.
2. Install library **Adafruit NeoPixel**.
3. Open `esp8266/Pixel25_MobileOnly.ino`. Keep `webui.h` in the same sketch folder.
4. Choose your ESP8266 board and USB COM port, then upload.
5. Open Serial Monitor at 115200 baud if you want to see the hotspot status.
6. On your phone, connect to `Pixel25-LED` Wi-Fi (password `pixel2588`), then open `http://192.168.4.1`.

## Features
- Mobile browser photo upload and 5x5 downsample preview-to-LED
- Video playback streamed as 5x5 frames from the phone browser
- Brightness / contrast / color controls, dither toggle, video speed
- Running text with scroll speed and color
- Adjustable display width and height (1–5 columns / rows)
- Custom 25-LED physical mapping editor with persistent EEPROM save
- Connection status checks the ESP8266 instead of showing a fake connected state
- Phone-browser settings saved in local storage
- Manual paint grid
- LED data pin: D4 (GPIO2)
- 25 WS2812B LEDs; firmware applies serpentine mapping (even rows left-to-right, odd rows right-to-left)

## Wiring
- WS2812B DIN -> ESP8266 D4 (GPIO2), preferably through a 330–470 ohm series resistor
- LED strip 5V -> external 5V supply sized for the strip
- LED strip GND -> supply GND and ESP8266 GND (common ground)
- Do not power all 25 pixels from the ESP8266 board's 5V/USB pin; use a suitable external 5V source.

## Notes / limitations
- Custom mapping is stored in ESP8266 EEPROM; browser control settings are stored on the phone browser. Clearing browser data resets those settings.
- Firmware compilation and physical LED testing are not performed in this environment; JavaScript syntax is checked.
- The display is only 5x5 pixels, so images/videos are very low resolution.
- Video is processed on the phone and frames are sent locally over Wi-Fi to the ESP.
- Connect to the ESP-created Wi-Fi to use this app; internet may be unavailable while connected.
- A phone may ask for permission to access selected media. Choose the file from Photos/Gallery.
- Text supports a compact bitmap font for uppercase A–Z, digits, and basic punctuation; lowercase input is shown as uppercase.
