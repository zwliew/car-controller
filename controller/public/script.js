window.onload = () => {
  const socket = new WebSocket('ws://localhost:8080');
  socket.addEventListener('open', () => {
    console.log('Connected to WebSockets.');
  })

  function moveFront() {
    const message = {
      type: 'direction',
      value: 'front'
    };
    socket.send(JSON.stringify(message));
    directionText.innerText = 'Direction: front';
    console.log('Direction: front');
  }

  const leftBtn = document.getElementById('left-btn');
  const rightBtn = document.getElementById('right-btn');
  const speedSlider = document.getElementById('speed-slider');
  const speedText = document.getElementById('speed-text');
  const directionText = document.getElementById('direction-text');

  let leftPressed = false;
  let rightPressed = false;

  leftBtn.addEventListener('mousedown', () => {
    leftPressed = true;
    const message = {
      type: 'direction',
      value: 'left',
    };
    socket.send(JSON.stringify(message));
    directionText.innerText = 'Direction: left';
    console.log('Direction: left');
  });

  rightBtn.addEventListener('mousedown', () => {
    rightPressed = true;
    const message = {
      type: 'direction',
      value: 'right',
    };
    socket.send(JSON.stringify(message));
    directionText.innerText = 'Direction: right';
    console.log('Direction: right');
  });

  leftBtn.addEventListener('mouseup', () => {
    leftPressed = false;
    if (!leftPressed && !rightPressed) {
      moveFront();
    }
  });

  rightBtn.addEventListener('mouseup', () => {
    rightPressed = false;
    if (!leftPressed && !rightPressed) {
      moveFront();
    }
  });

  speedSlider.addEventListener('change', (event) => {
    const speed = event.target.value;
    const message = {
      type: 'speed',
      value: speed
    }
    socket.send(JSON.stringify(message));
    speedText.innerText = `Speed: ${speed}`;
    console.log(`Speed: ${speed}`);
  });
};
