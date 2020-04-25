
class Room{
    constructor(title, password, owner, isPublic=true, capacity=4){
        this.title = title;
        this.password = password;
        this.owner = owner;
        this.isPublic = isPublic;
        this.capacity = capacity;
        this._userList = new Array(capacity);
        this.appendUser(owner);
    }

    get userList(){
        return this._userList;
    }

    appendUser(userId){
        this._userList.push(userId);
    }
}

module.exports = Room;