const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const dgram = require('dgram');
const os = require('os');
const path = require('path');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server, maxPayload: 1024 * 1024 });
const txUdp = dgram.createSocket('udp4');
const discoveryUdp = dgram.createSocket('udp4');
const PORT = Number(process.env.PORT || 3000);
const ESP_UDP_PORT = 4210;
const DISCOVERY_PORT = 4211;
let espIP = null;
let lastSeen = 0;

app.use(express.static(path.join(__dirname, 'public')));
function lanIPs(){ return Object.values(os.networkInterfaces()).flat().filter(x => x && x.family === 'IPv4' && !x.internal).map(x => x.address); }
app.get('/api/status', (_, res) => res.json({node: true, espConnected: !!espIP && Date.now()-lastSeen < 10000, espIP: espIP && Date.now()-lastSeen < 10000 ? espIP : null, lastSeen, udpPort: ESP_UDP_PORT, discoveryPort: DISCOVERY_PORT, lanIPs: lanIPs()}));

discoveryUdp.on('message', (msg, rinfo) => {
  if (msg.toString().trim() === 'PXDISC') {
    espIP = rinfo.address;
    lastSeen = Date.now();
  }
});
discoveryUdp.on('error', err => console.error('Discovery UDP error:', err.message));
discoveryUdp.bind(DISCOVERY_PORT, '0.0.0.0', () => console.log(`ESP auto-discovery listening on UDP ${DISCOVERY_PORT}`));

wss.on('connection', ws => {
  ws.on('message', (data, isBinary) => {
    if (isBinary) {
      if (data.length === 79 && data.toString('ascii', 0, 4) === 'PX25' && espIP && Date.now()-lastSeen < 10000) txUdp.send(data, ESP_UDP_PORT, espIP);
      return;
    }
    try {
      const msg = JSON.parse(data.toString());
      if (msg.type === 'brightness') {
        const b = Math.max(0, Math.min(255, Number(msg.value) || 0));
        const packet = Buffer.alloc(5); packet.write('PXL!', 0, 'ascii'); packet[4] = b;
        if (espIP && Date.now()-lastSeen < 10000) txUdp.send(packet, ESP_UDP_PORT, espIP);
      }
      if (msg.type === 'clear') {
        const packet = Buffer.alloc(79); packet.write('PX25', 0, 'ascii');
        if (espIP && Date.now()-lastSeen < 10000) txUdp.send(packet, ESP_UDP_PORT, espIP);
      }
    } catch (e) { console.error('WebSocket message error:', e.message); }
  });
});
server.listen(PORT, '0.0.0.0', () => {
  const urls = lanIPs().map(ip => `http://${ip}:${PORT}`);
  console.log(`Pixel25 Studio started: http://localhost:${PORT}`);
  console.log('Mobile/PC access on same Wi-Fi:', urls.join(' | ') || '(no LAN IP found)');
  console.log('ESP auto-discovery enabled. No IP entry needed.');
});
