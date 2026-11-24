# Pixel25 Studio (25× WS2812B + ESP8266)

Responsive browser control for a 5×5 pixel display. Upload image/video, preview 25 pixels, edit LED mapping, paint pixels, and control brightness. Node.js bridge sends pixel frames from your PC to the ESP8266 via UDP.

## Important: Vercel deployment limitation
Vercel can host the static UI, but the actual LED control requires the Node.js bridge to be running on a computer on the same LAN as the ESP8266. Vercel serverless functions do not provide the persistent UDP bridge needed to send frames to a private LAN ESP. The hosted page alone will not control the LEDs. Use the local Node bridge for functional playback.

## Run locally (full functionality)
1. Install Node.js LTS.
2. Flash `esp8266/Pixel25_ESP8266.ino` with your Wi-Fi SSID/password. Install the Adafruit NeoPixel Arduino library.
3. In this project folder run `npm install` then `npm start`.
4. On the PC open `http://localhost:3000`.
5. On phones/tablets on the same Wi-Fi, open the LAN URL printed in the terminal.
6. Enter ESP IP (no `:3000`) and connect.

## GitHub upload
Create a repository, upload the files/folders in this project, or push with git. Do not upload `node_modules`.


## Vercel (static preview only)
- Import this GitHub repository in Vercel.
- Set the Root Directory to the project root.
- Vercel will serve the `public` folder as a static website using `vercel.json`.
- The hosted static page is for UI preview; the local Node bridge is still required for ESP playback.


## Wiring
- WS2812B DIN → ESP8266 D4 / GPIO2
- LED strip 5V → external regulated 5V supply
- LED strip GND and ESP GND → common ground
- Recommended 330Ω resistor in series on data line
- Provide adequate 5V current for 25 LEDs; avoid powering strip from the ESP board regulator.
