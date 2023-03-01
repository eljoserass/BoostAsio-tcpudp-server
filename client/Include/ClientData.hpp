#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;


namespace ClientController {

class ClientData {
    public:

        ClientData();
        ~ClientData(){};

        std::string currentRoomName;
        std::string currentRoomID;
        std::vector<std::tuple <std::string, bool>> currentRoomPlayers;
        std::vector<std::string> currentRoomPlayersName;
        std::vector<std::tuple <std::string, std::string>> currentAvailableRooms;
        std::string currentGamePort;
        std::string clientId;
        std::string clientName;
        std::string udpPort;
        bool isInGame;
        bool isReady;
        bool isInRoom;
        bool isConnected;

};

}   
