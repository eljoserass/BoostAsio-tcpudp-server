#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <msgpack.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

namespace ClientController {

    class UDPClient {
        public:
            UDPClient(std::string ip, std::string port);

            int sendCommand(std::string command);
            void receive();
            void join();
            std::string passToBinary(std::string command);


            boost::asio::io_service io_service;
            udp::socket socket;
            udp::resolver resolver;
            udp::resolver::query query;
            udp::endpoint receiver_endpoint;
            udp::endpoint sender_endpoint;
            std::thread receiver_thread;
            std::shared_ptr<std::string> response;

    };
}