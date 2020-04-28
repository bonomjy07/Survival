console.log('Hello, Im Server');
 
server = require('http').Server();
var socketIO = require('socket.io');
const Room = require('./server/lib/room');
var io = socketIO.listen(server);
var host = '';
var playerList = [];
var roomList = [];

// connection
io.on('connection', function (socket) {
    console.log("connected : ", socket.id);
    playerList.push(socket.id);
 
    if (host == '')  // First one is the 'Host'
        host = socket.id;
 
    // socket.emit('requestPlayerID', { HostID: host, MyID: socket.id });
	
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

    socket.on('NewPawnDirection', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('NewPawnDirection', data);
    });

    socket.on('SpriteDeletion', function (data) {
        data = JSON.parse(data);
        socket.broadcast.emit('SpriteDeletion', data);
    });

    socket.on('make-room', function(data){
        data = JSON.parse(data);
        console.log(data);
        newRoom = new Room(data.title, data.password, socket.id);
        //roomList[newRoom.title] = newRoom;
        roomList[newRoom.title] = JSON.stringify(newRoom);
        socket.join(newRoom.title, (err) => {
            if (err !== null){
                console.log(err);
                return;
            }
            console.log('maked new room is ' + newRoom.title);
        });
        console.log(roomList[newRoom.title]);
    });

	// Give room list to client
	socket.on('refresh-rooms', function(data) {
		var rooms=[];
		for (let [key, value] of Object.entries(roomList)) {
			rooms.push(value);
		}
		rooms = JSON.parse(rooms);
		socket.emit('roomlist', rooms);
	});

    socket.on('disconnect', function () {
        console.log("["+socket.id+"]"+ " is " + "disconnected");
        if ( socket.id === host ) {
            socket.disconnect();
            host = '';
        }
    });
});
server.listen(8080);
