#pragma once

#include "UDPClient.hpp"

namespace ClientController {

    class AvailableRooms {
        public:

            AvailableRooms(const char* ipParam, const char* portParam, const char* nameParam, const char* difficultyParam, const char *maxPlayersParam) : 
            ip(ipParam), port(portParam), name(nameParam), difficulty(difficultyParam), max_players(maxPlayersParam) {
            }

            std::string ip;
            std::string port;
            std::string name;
            std::string difficulty;
            std::string max_players;
    };

    class ClientServer {
        public:
            ClientServer();

            void startGame(int pos);
            void listen(void) ;
            void parse_from_cli(void) ;
            void join(void);

            UDPClient *udpClient;
            std::vector<AvailableRooms*> available_rooms;
    };
}