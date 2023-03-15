#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <bitset>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

namespace ClientController {

    class UDPClient {
        public:
            UDPClient(std::string ip, std::string port);

            int sendCommand(std::string command);
            void receive();
            void join();
            std::string passStringToBinary(const std::string &str);
            void close();

            std::size_t total_bytes_sent_in_second = 0;
            std::shared_ptr<bool> isOpen;
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