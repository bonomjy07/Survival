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
 
    socket.emit('requestPlayerID', { HostID: host, MyID: socket.id });
	
	socket.on('newPlayer', function(data) {
		data = JSON.parse(data);
		io.to(host).emit('newPlayer', data);
	});

	socket.on('playerList', function(data) {
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
    
    socket.on('pawnMove', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('pawnMove', data);
    });

    socket.on('NewUnitHealth', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('NewUnitHealth', data);
    });

    socket.on('SpriteDeletion', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('SpriteDeletion', data);
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
