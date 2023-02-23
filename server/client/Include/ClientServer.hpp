#pragma once

#include "TCPClient.hpp"
#include "UDPClient.hpp"

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
        void speak_to_UDPserver(udp::socket& socket, udp::endpoint& receiver_endpoint);

        std::shared_ptr<std::string> gameState;
        bool isInGame;
        std::shared_ptr<bool> isUDPRunning_; // use this to pass it as a parameter to the udp loop in order to stop it accordingly
        std::shared_ptr<bool> isTCPRunning_;
        std::thread parse_response_thread;
        TCPClient *tcpClient_;
        std::string udpPort_;
        std::string host_;
        std::string uuid;
        std::string current_room_uuid;
        std::shared_ptr<ClientData> clientData;

        /*
        agregar resto de metodos para interacturar con los rooms, atributos para guardar rooms, variables para saber en que estado se está
        */
    private:
        UDPClient *udpClient_;
        std::string tcpPort_;
};