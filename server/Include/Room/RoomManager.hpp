#pragma once
#include "GameManager.hpp"

/*
The class Client -> MainServer interacts with room logic and data about rooms in the server.
Gives commands to cahnge data in a room and start game, then logic inside of a game is handled directly with Game class and UDP sockets
*/

class RoomManager
{
    public:
        RoomManager();
        ~RoomManager() {};
        void createRoom(const string roomName);
        void deleteRoomById(boost::uuids::uuid id);
        vector<string> getRoomsName();
        vector<string> getPlayersByRoomId(boost::uuids::uuid roomId);
        boost::uuids::uuid addPlayer(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string playerName);
        void addPlayerToRoom(boost::uuids::uuid roomId, boost::uuids::uuid playerId);
        void removePlayerFromRoom(boost::uuids::uuid playerId);
        void startGame(void);
        bool isRoomReadyByRoomId(boost::uuids::uuid roomId);
        vector<boost::uuids::uuid> getPlayersIdByRoomId(boost::uuids::uuid roomId);
        void sendInfoByPlayersId(vector<boost::uuids::uuid> playerIds, const string &message);
        vector<Player> _players;
        GameManager *_GameManager;
        vector<Room> _rooms;
        vector<string> _roomNames;
        vector<unsigned int> _roomIds;
};