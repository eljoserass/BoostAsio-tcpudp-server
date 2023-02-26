#pragma once

#include "./ClientData.hpp"

class TCPClient {
    public:
        TCPClient(std::string host, int port);
        void run(std::shared_ptr<bool>& isTcpRunning, std::shared_ptr<ClientData>& clientData);
        void setIsConnected(bool value);
        void joinThread();
        std::string getCommandFromServer();
        boost::asio::io_service _io_service;
        std::shared_ptr<bool> isTcpRunning_;
        std::shared_ptr<std::string> commandFromServer;
        tcp::socket _socket;
        std::thread listen_thread_;
        bool _isConnected;
        std::string gamePort_;

        void _send(const std::string &message);
        void _receive();
};