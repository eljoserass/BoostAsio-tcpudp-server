#include "../Include/Room/RoomManager.hpp"
using namespace Server;

RoomManager::RoomManager() : _players()
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

vector<string> RoomManager::getPlayersByRoomId(boost::uuids::uuid roomId)
{
    vector<string> result;

    for (auto &player: _players) {
        if (player.currentRoomId == roomId)
            result.push_back(player.getPlayerName());
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

boost::uuids::uuid RoomManager::addPlayer(string &playerName)
{
   
    // Player *playerptr = new Player(socket, playerName);
    Player *player = new Player(playerName);
    _players.push_back(*player);
    std::cout << "addPlayer" << std::endl;
    return _players.back().getPlayerId();
    // return _players[_players.size() -1]->getPlayerId();
}

void RoomManager::addPlayerToRoom(boost::uuids::uuid roomId, boost::uuids::uuid playerId)
{
    for (auto &player: _players) {
        if (player.getPlayerId() == playerId)
            player.currentRoomId = roomId;
    }
}

void RoomManager::removePlayerFromRoom(boost::uuids::uuid playerId)
{
    for (auto &player: _players) {
        if(player.getPlayerId() == playerId) {
            player.currentRoomId = boost::uuids::nil_uuid();
        }
    }
    // for (int i = 0; i < _players.size(); i++) {
    //     if (_players[i]->getPlayerId() == playerId)
    //         _players[i]->currentRoomId = boost::uuids::nil_uuid();
    // }
}

bool RoomManager::isRoomReadyByRoomId(boost::uuids::uuid roomId)
{
    int players_ready = 0;
    int players_in_room = 0;
    

    for (auto &player: _players) {
        if (player.currentRoomId == roomId) {
            players_in_room++;
        }
    }
    // for (int i = 0; i < _players.size(); i++) {
    //     if (_players[i]->currentRoomId == roomId)
    //         players_in_room++;
    // }
    for (auto &player: _players) {
        if (player.currentRoomId == roomId) {
            if (player.getIsReady() == true)
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

    for (auto &player: _players) {
        if (player.currentRoomId == roomId)
            playerIds.push_back(player.getPlayerId());
    }
    return playerIds;
}

void RoomManager::sendInfoByPlayersId(vector<boost::uuids::uuid> playerIds, const string &message)
{
    for (int i = 0; i < playerIds.size(); i++) {
        for (auto &player: _players) {
            boost::system::error_code error;
            boost::asio::write(*player.socket, boost::asio::buffer(message), error);
            if (error)
                std::cerr << "Error sending message to client: " << error.message() << endl;
        }
    }
}

void RoomManager::setPlayerReady(boost::uuids::uuid playerId)
{
    for (auto &player: _players) {
        if (player._id == playerId)
            player.setIsReady();
    }
}

void RoomManager::setPlayerNotReady(boost::uuids::uuid playerId)
{
    for (auto &player: _players) {
        if (player._id == playerId)
            player.setIsNotReady();
    }
}

vector<tuple<boost::uuids::uuid, string>> RoomManager::getRoomsInfo(void)
{
    vector<tuple<boost::uuids::uuid, string>> _roomsInfo;

    for (int i = 0; i < _rooms.size(); i++) {
        _roomsInfo.push_back(_rooms[i].getRoomInfo());
    }
    return _roomsInfo;
}

vector<tuple<boost::uuids::uuid, string>> RoomManager::getPlayersInfoByRoomId(boost::uuids::uuid roomId)
{
    vector<tuple<boost::uuids::uuid, string>> players_info;
    for (auto &player: _players) {
        if (player.currentRoomId == roomId)
            _playerInfo.push_back(player.getPlayerInfo());
    }
    return players_info;
}