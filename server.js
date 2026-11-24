const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const dgram = require('dgram');
const os = require('os');
const path = require('path');
const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server, maxPayload: 1024 * 1024 });
const udp = dgram.createSocket('udp4');
const PORT = Number(process.env.PORT || 3000);
let espIP = '192.168.1.100';
const udpPort = 4210;
app.use(express.static(path.join(__dirname, 'public')));
app.get('/api/status', (_, res) => res.json({ udpPort, espIP, lanIPs: Object.values(os.networkInterfaces()).flat().filter(x => x && x.family === 'IPv4' && !x.internal).map(x => x.address) }));
wss.on('connection', ws => {
  ws.on('message', (data, isBinary) => {
    if (isBinary) {
      if (data.length === 79 && data.toString('ascii', 0, 4) === 'PX25') udp.send(data, udpPort, espIP);
      return;
    }
    try {
      const msg = JSON.parse(data.toString());
      if (msg.type === 'setIP' && /^\d{1,3}(\.\d{1,3}){3}$/.test(msg.ip)) espIP = msg.ip;
      if (msg.type === 'brightness') {
        const b = Math.max(0, Math.min(255, Number(msg.value) || 0));
        const packet = Buffer.alloc(5); packet.write('PXL!', 0, 'ascii'); packet[4] = b; udp.send(packet, udpPort, espIP);
      }
      if (msg.type === 'clear') { const packet = Buffer.alloc(79); packet.write('PX25', 0, 'ascii'); udp.send(packet, udpPort, espIP); }
    } catch (e) { console.error('WebSocket message error:', e.message); }
  });
});
server.listen(PORT, '0.0.0.0', () => {
  const ips = Object.values(os.networkInterfaces()).flat().filter(x => x && x.family === 'IPv4' && !x.internal).map(x => `http://${x.address}:${PORT}`);
  console.log(`Pixel25 Player started: http://localhost:${PORT}`);
  console.log('Mobile/PC on same Wi-Fi can use:', ips.join(' | ') || '(no LAN IP found)');
});
