#pragma once

#include "../Player/Player.hpp"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::tuple;
using std::get;
using std::make_pair;

/*
Structure of a Room.
Handler of the interactions made by SFML/Player and info inside rooms
*/

namespace Server {
    class Room {
        public:
            Room(const string roomName);
            // ~Room();
            void setRoomId(const string roomName);
            void setRoomName(const string &roomName);
            void setIsGameStarted(bool is);
            bool getIsGameStarted();
            string getRoomName(void);
            tuple<boost::uuids::uuid, string> getRoomInfo(void);
            // void removePlayerByRoomId(unsigned int roomId, const string playerName);
            // void addPlayerByRoomId(unsigned int roomId, const string playerName);

        private:
            bool _isGameStarted;
            string _roomName;
            boost::uuids::uuid _id;
            // vector<tuple<int, string>> _playerInfo;
            tuple<boost::uuids::uuid, string> _roomInfo;
            vector<string> _playersByRoomId;
    };
}