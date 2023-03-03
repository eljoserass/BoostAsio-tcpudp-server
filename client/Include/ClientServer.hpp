#pragma once

#include "./TCPClient.hpp"
#include "./UDPClient.hpp"


namespace ClientController {


class ClientServer {

    public:
        ClientServer();
        ~ClientServer() {};
        void startUdp(std::string port);
        void startTcp();
        void joinThread(void);
        void run();
        void setUdpPort(const char *udpPort);
        void setTcpPort(const char *tcpPort);
        void setHost(const char *host);
        void speak_to_UDPserver(udp::socket& socket, udp::endpoint& receiver_endpoint, std::string &message);
        std::string findRoomUuidByName(std::string roomName) {
            for (int i =0; i < clientData->currentAvailableRooms.size(); i++) {
                if (roomName == std::get<0>(clientData->currentAvailableRooms[i])) {
                    return std::get<1>(clientData->currentAvailableRooms[i]);
                }
                
            }
            return ("none");
        }
        void sendUdpCommand(std::string command) {
            udpClient_->sendCommand(command);
        }
        std::shared_ptr<std::string> gameState;
        bool isInGame;
        std::shared_ptr<bool> isUDPRunning_; // use this to pass it as a parameter to the udp loop in order to stop it accordingly
        std::shared_ptr<bool> isTCPRunning_;
        std::thread parse_response_thread;
        TCPClient *tcpClient_;
        std::string udpPort_;
        std::string host_;
        std::string playerName;
        std::string uuid;
        std::string current_room_uuid;
        std::shared_ptr<ClientData> clientData;

        void setPlayerName(std::string& name) {
            playerName = name;
        }

        int fetchPlayersInRoom() {
            tcpClient_->_send("players_info;" + clientData->currentRoomID + "|");
            return (0);
        }


        int fetchRooms() {
            tcpClient_->_send("room_info|");
            std::cout << "fetch rooms" << std::endl;
            return (0);
        }
        int createRoom(std::string room) {
            // tcpClient_->_send("create_room;room1");
            // clientData->currentAvailableRooms.push_back(room);
            tcpClient_->_send("create_room;"+room + "|");
            return (0);
        }
        int enterRoom(std::string roomName) {
            // clientData->currentRoomName = roomName;
            if (findRoomUuidByName(roomName) == "none") 
                return -1;
            if (clientData->isInRoom)
                return -1;
            tcpClient_->_send("add_player_room;" + findRoomUuidByName(roomName) + ":" + clientData->clientId  + "|");
            clientData->currentRoomName = roomName;
            clientData->currentRoomID = findRoomUuidByName(roomName);
            clientData->isInRoom = true;
            return (0);
        }
        int leaveRoom() {
            clientData->currentRoomName = "";
            tcpClient_->_send("remove_player_room;" + clientData->clientId  + "|");
            setReady(false);
            return (0);
        }
        int setReady(bool val) {
            if (clientData->isInRoom) {
                clientData->isReady = val;
                if (val)
                    tcpClient_->_send("player_ready;" + clientData->clientId  + "|");
                else
                    tcpClient_->_send("player_not_ready;" + clientData->clientId  + "|");
                return 0;
            }
            return 1;
        }

        int setPlayerName(std::string name) {
            tcpClient_->_send("new_player;" + name + "|");
            clientData->clientName = name;
            return (0);
        }

        int isCurrentRoomReady(void) {
            tcpClient_->_send("is_room_ready;" + clientData->currentRoomID + "|");
            return (0);
        }

        int deleteRoomByName(std::string roomName) {
            if (findRoomUuidByName(roomName) != "none")
                tcpClient_->_send("delete_room;" + findRoomUuidByName(roomName) + "|");
            return (0);
        }
        /*
        agregar resto de metodos para interacturar con los rooms, atributos para guardar rooms, variables para saber en que estado se está
        */
    private:
        UDPClient *udpClient_;
        std::string tcpPort_;
};

}