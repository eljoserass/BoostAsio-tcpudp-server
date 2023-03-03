#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

void run_receive_thread(udp::socket& socket, udp::endpoint& sender_endpoint, std::shared_ptr<std::string>& response) {
    while (true) {
        boost::array<char, 1028> recv_buf;
        
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        *response = std::string(recv_buf.data(), len);

        /// REMOVE WHEN NOT USING CLI!
        std::cout.write(recv_buf.data(), len);
        std::cout.write("\n", 1);
    }
}

namespace ClientController {

    class UDPClient {
        public:
            UDPClient(std::string ip, std::string port);

            int sendCommand(std::string command);
            void receive();
            void join();


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