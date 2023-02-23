#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

class ClientData {
    public:

        ClientData();
        ~ClientData(){};

        std::string currentRoomName;
        std::string currentRoomID;
        std::vector<std::string> currentRoomPlayers;
        std::vector<std::string> currentAvailableRooms;
        std::string currentGamePort;
        std::string clientId;
        std::string clientName;
        bool isInGame;
};