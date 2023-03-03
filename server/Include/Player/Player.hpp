#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::tuple;

/*
Class that will contain all info necessary about a client that is connected
*/

namespace Server {
    class Player {
        public:
            Player(string& playerName);
            // ~Player(void);
            void setPlayerId(const string playerName);
            boost::uuids::uuid getPlayerId(void);
            void setPlayerName(const string &playerName);
            string getPlayerName(void);
            void setIsReady(void);
            void setIsNotReady(void);
            bool getIsReady(void);
            void setPlayerInfo(const string &playerName, boost::uuids::uuid id);
            tuple<boost::uuids::uuid, string> getPlayerInfo(void);
            // bool getIsLogOut(void);
            // void setIsLogOut(void);
            // const string getPlayerName();
            boost::uuids::uuid currentRoomId;
            std::shared_ptr<boost::asio::ip::tcp::socket> socket;
            boost::uuids::uuid _id;

        private:
            string _playerName;
            bool _isReady;
            bool _isLogOut;
            tuple<boost::uuids::uuid, string> _playerInfo;
    };
}