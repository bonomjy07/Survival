console.log('Hello, Im Server');
 
server = require('http').Server();
var socketIO = require('socket.io');
var io = socketIO.listen(server);
var host = '';
var playerList = [];

// connection
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
 
    // Send guest action data to host
    socket.on('action', function(data) {
        data = JSON.parse(data);
        io.to(host).emit('action', data);
    });
    socket.on('doAction', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('doAction', data);
    });
    
    // Host sends to all clients which pawn moves
    socket.on('pawnMove', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('pawnMove', data);
    });

    socket.on('disconnect', function () {
        console.log("disconnect");
        if ( socket.id === host ) {
            socket.disconnect();
            host = '';
        }
    });
});
server.listen(8080);