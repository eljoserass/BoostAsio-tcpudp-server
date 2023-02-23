#include "../Include/Room/RoomManager.hpp"

RoomManager::RoomManager()
{
    _GameManager = new GameManager();
}

void RoomManager::createRoom(const string roomName)
{
    _rooms.push_back(Room(roomName));
}

void RoomManager::deleteRoomById(boost::uuids::uuid id)
{
    for (int i = 0; i < _rooms.size(); i++) {
        if (get<0>(_rooms[i].getRoomInfo()) == id)
            _rooms.erase(_rooms.begin() + i);
    }
}

vector<string> RoomManager::getRoomsName()
{
    for (unsigned int i = 0; i < _rooms.size(); i++) {
        _roomNames.push_back(_rooms[i].getRoomName());
    }
    return _roomNames;
}

vector<string> RoomManager::getPlayersByRoomId(boost::uuids::uuid roomId)
{
    vector<string> result;

    for (int i = 0; i < _players.size(); i++) {
        if (_players[i].currentRoomId == roomId)
            result.push_back(_players[i].getPlayerName());
    }
    return result;
}

void RoomManager::startGame()
{
    for (int i = 0; i < _rooms.size(); i++) {
        if (isRoomReadyByRoomId(get<0>(_rooms[i].getRoomInfo())) == true) {
            int port = _GameManager->startGame(get<1>(_rooms[i].getRoomInfo()));
            string gameStarted = "gameStarted";
            string stringPort = std::to_string(port);
            string command = gameStarted + ";" + stringPort;
            sendInfoByPlayersId(getPlayersIdByRoomId(get<0>(_rooms[i].getRoomInfo())), command);
        }
    }
}

boost::uuids::uuid RoomManager::addPlayer(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string playerName)
{
    Player player(socket, playerName);
    _players.push_back(player);
    
    return player.getPlayerId();
}

void RoomManager::addPlayerToRoom(boost::uuids::uuid roomId, boost::uuids::uuid playerId)
{
    for (int i = 0; i < _players.size(); i++) {
        if (_players[i].getPlayerId() == playerId)
            _players[i].currentRoomId = roomId;
    }
}

void RoomManager::removePlayerFromRoom(boost::uuids::uuid playerId)
{
    for (int i = 0; i < _players.size(); i++) {
        if (_players[i].getPlayerId() == playerId)
            _players[i].currentRoomId = boost::uuids::nil_uuid();
    }
}

bool RoomManager::isRoomReadyByRoomId(boost::uuids::uuid roomId)
{
    int players_ready = 0;
    int players_in_room = 0;
    
    for (int i = 0; i < _players.size(); i++) {
        if (_players[i].currentRoomId == roomId)
            players_in_room++;
    }
    for (int j = 0; j < _players.size(); j++) {
        if (_players[j].currentRoomId == roomId) {
            if (_players[j].getIsReady() == true)
                players_ready++;
            if (players_ready == players_in_room)
                return true;
        }
    }
    return false;
}

vector<boost::uuids::uuid> RoomManager::getPlayersIdByRoomId(boost::uuids::uuid roomId)
{
    vector<boost::uuids::uuid> playerIds;

    for (int i = 0; i < _players.size(); i++) {
        if (_players[i].currentRoomId == roomId)
            playerIds.push_back(_players[i].getPlayerId());
    }
    return playerIds;
}

void RoomManager::sendInfoByPlayersId(vector<boost::uuids::uuid> playerIds, const string &message)
{
    for (int i = 0; i < playerIds.size(); i++) {
        for (int i = 0; i < _players.size(); i++) {
            boost::system::error_code error;
            boost::asio::write(*_players[i].socket, boost::asio::buffer(message), error);
            if (error)
                std::cerr << "Error sending message to client: " << error.message() << endl;
        }
    }
}