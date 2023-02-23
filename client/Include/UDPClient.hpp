#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

class UDPClient {
    public:
        UDPClient(std::string host, std::string port, std::shared_ptr<std::string>& gameState);
        void run(std::shared_ptr<std::string>& gameState, std::shared_ptr<bool> running);
        void joinThread();

    private:
        boost::asio::io_context io_context_;
        std::thread listen_thread_;
        udp::resolver resolver_;
        udp::endpoint receiver_endpoint_;
        udp::socket socket_;

};