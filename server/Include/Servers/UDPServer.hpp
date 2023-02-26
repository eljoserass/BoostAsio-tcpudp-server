#pragma once

#include <iostream>
#include <set>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "../Room/Room.hpp"
#include "../Player/Player.hpp"

using boost::asio::ip::udp;

class UDPServer {
    public:
        UDPServer(int port);
        void send_to_all(const std::string &message);
        void run(void);
        // void remove_players();

        std::shared_ptr<std::string> clientMessage_;
        boost::asio::io_context io_context;
    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        boost::array<char, 1024> recv_buffer_;
        std::map<udp::endpoint, bool> clients_;
        std::mutex mtx;
    
        void start_receive();
        void handle_receive(const boost::system::error_code& error, std::size_t received);

        void handle_send(boost::shared_ptr<std::string> message,
                        const boost::system::error_code& error,
                        std::size_t bytes) {};
};
