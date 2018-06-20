const fs = require('fs');
const http = require('http');
const WebSocket = require('ws');
const serveStatic = require('serve-static');
const finalHandler = require('finalhandler');
const net = require('net');

const serve = serveStatic('public', {'index': [ 'index.html' ]});
const server = new http.createServer((req, res) => {
  serve(req, res, finalHandler(req, res));
});

const client = new net.Socket();
client.connect(2001, '192.168.1.1', () => {
  console.log('Bot connected.');
});

const wss = new WebSocket.Server({ server });
wss.on('connection', (ws) => {
  console.log('Web controller connected.');
  ws.on('message', (message) => {
    const { type, value } = JSON.parse(message);
    switch (type) {
      case 'speed':
        if (value > 255 || value < -255) {
          console.log(`Invalid speed received: ${value}`);
          return;
        }
        console.log(`New speed: ${value}`);
        client.write(`s_${value}`);
        break;
      case 'direction':
        if (value !== 'left' && value !== 'right' && value !== 'front') {
          console.log(`Invalid direction received: ${value}`);
          return;
        }
        console.log(`New direction: ${value}`);
        client.write(`d_${value[0]}`);
        break;
    }
  });
});

server.listen(8080);
