#pragma once
#include "../Game/GameManager.hpp"

/*
The class Client -> MainServer interacts with room logic and data about rooms in the server.
Gives commands to cahnge data in a room and start game, then logic inside of a game is handled directly with Game class and UDP sockets
*/

namespace Server {
    class RoomManager
    {
        public:
            RoomManager();
            ~RoomManager() {};
            void createRoom(const string roomName);
            void deleteRoomById(boost::uuids::uuid id);
            vector<tuple<boost::uuids::uuid, string>> getRoomsInfo(void);
            vector<string> getPlayersByRoomId(boost::uuids::uuid roomId);
            boost::uuids::uuid addPlayer(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string playerName);
            void addPlayerToRoom(boost::uuids::uuid roomId, boost::uuids::uuid playerId);
            void removePlayerFromRoom(boost::uuids::uuid playerId);
            void startGame(void);
            bool isRoomReadyByRoomId(boost::uuids::uuid roomId);
            vector<boost::uuids::uuid> getPlayersIdByRoomId(boost::uuids::uuid roomId);
            void sendInfoByPlayersId(vector<boost::uuids::uuid> playerIds, const string &message);
            void setPlayerReady(boost::uuids::uuid playerId);
            void setPlayerNotReady(boost::uuids::uuid playerId);
            vector<tuple<boost::uuids::uuid, string>> getPlayersInfoByRoomId(boost::uuids::uuid roomId);

            vector<Player*> _players;
            GameManager *_GameManager;

        private:
            vector<Room> _rooms;
            vector<tuple<boost::uuids::uuid, string>> _playerInfo;
    };
}
