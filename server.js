console.log('Hello, Im Server');
 
server = require('http').Server();
var socketIO = require('socket.io');
var io = socketIO.listen(server);
var host = '';
var playerList = [];
//io.sockets.on('connection', function (socket) {
io.on('connection', function (socket) {
    console.log("connected : ", socket.id);
    playerList.push(socket.id);
 
    if (host == '') 
        host = socket.id;
 
    // Sends IDs to new clinet
    socket.emit('requestPlayerID', { HostID: host, MyID: socket.id });
    // Informs host that new client is connected
    io.to(host).emit('newPlayer', { ID: socket.id });
    // Host sends player list to new client
    socket.on('playerList', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('playerList', data);
    });
 
 
    // Client pressed movemet input
    socket.on('movePressed', function (data) {
        data = JSON.parse(data);
        io.to(host).emit('movePressed', data);
    });
 
    // Client released movement input
    socket.on('moveReleased', function (data) {
        data = JSON.parse(data);
        io.to(host).emit('moveReleased', data);
    });
    
 
    // Host sends to all clients which pawn moves
    socket.on('pawnMove', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('pawnMove', data);
    });
 
    socket.on('disconnect', function () {
        console.log("disconnect");
    });
});
server.listen(8080);