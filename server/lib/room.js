
class Room{
    constructor(title, password, owner, isPublic=true, capacity=4){
        this.title = title;
        this.password = password;
        this.owner = owner;
        this.isPublic = isPublic;
        this.capacity = capacity;
    }
}

module.exports = Room;