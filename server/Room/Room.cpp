#include "../Include/Room/Room.hpp"
using namespace Server;

Room::Room(const string roomName)
{
    setRoomName(roomName);
    setRoomId(roomName);
}

void Room::setRoomName(const string &roomName)
{
    _roomName = roomName;
}

void Room::setRoomId(const string roomName)
{
    boost::uuids::random_generator generator;
    _id = generator();

    _roomInfo = make_pair(_id, roomName);
}

string Room::getRoomName()
{
    return _roomName;
}

tuple<boost::uuids::uuid, string> Room::getRoomInfo(void)
{
    return _roomInfo;
}

void Room::setIsGameStarted(bool is)
{
    _isGameStarted = is;
}

bool Room::getIsGameStarted()
{
    return _isGameStarted;
}

// vector<string> Room::getPlayersByRoomId(unsigned int roomId)
// {
//     for (const auto &info : _playerInfo) {
//         int playerId = get<0>(info);
//         if (playerId == roomId) {
//             _playersByRoomId.push_back(get<1>(info));
//         }
//     }
//     return _playersByRoomId;
// }

// void RoomManager::addPlayerByRoomId(unsigned int roomId, const string playerName)
// {
//     _rooms[roomId].addPlayer(roomId, playerName);
// }

// void RoomManager::removePlayerByRoomId(unsigned int roomId, const string playerName)
// {
//     _rooms[roomId].removePlayerById(roomId, playerName);
// }