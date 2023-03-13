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
            ClientServer(std::string host);
            ClientServer() {
                hostIp = "localhost";
                mainPort = "1234";
                available_rooms.push_back(new AvailableRooms(hostIp.c_str(), "1234", "room1", "easy", "4"));
                available_rooms.push_back(new AvailableRooms(hostIp.c_str(), "6969", "room2", "easy", "4"));
            };
            void setIp(std::string& ipHost) {
                hostIp = ipHost;
            };
            void setPort(std::string& mainPort) {
                mainPort = mainPort; 
            };
            void startGame(int pos);
            void startGame() {
                udpClient = new UDPClient(hostIp, mainPort);
            };
            void listen(void);
            void parse_from_cli(void) ;
            void join(void);
            void close() {
                udpClient->close();
            }
            std::string hostIp;
            std::string mainPort;
            UDPClient *udpClient;
            std::vector<AvailableRooms*> available_rooms;
    };
}