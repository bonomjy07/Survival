console.log('Hello, Im Server');
gameState =  { 
  player : {
  }
}

server = require('http').Server();
var socketIO = require('socket.io');
var io = socketIO.listen(server);
var host = '';

//io.sockets.on('connection', function (socket) {
io.on('connection', function (socket) {
  console.log("connected", socket.id);

  gameState.player[socket.id] = {};

  if (host == '')
    host = socket.id;

  socket.emit('requestPlayerID', { ID: socket.id, host: host });
  socket.broadcast.emit('newPlayer', { ID: socket.id });

  socket.on('movePressed', function (player) {
    console.log("movePressed:", socket.id);
    io.to(host).emit('movePressed', player);
  });
  /*
  socket.on('moveReleased', function (player) {
    io.to('${host}').emit('moveReleased', player);
  });

  socket.on('panwMove', function (player) {
    const obj = JSON.parse(player);
    io.to('${obj.ID}').emit('pawnMove', player);
  });
  */

  socket.on('disconnect', function () {
    console.log("disconnect");
  });
});

server.listen(8080);
