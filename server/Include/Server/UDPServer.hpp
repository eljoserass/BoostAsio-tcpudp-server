#pragma once

#include <iostream>
#include <set>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/algorithm/string.hpp>
#include <cstring>
#include <bitset>

using boost::asio::ip::udp;




namespace Server {
    class UDPServer {
        public:
            UDPServer(int port, boost::asio::io_context &io_context);
            void send_to_all(const std::string &message);
            std::shared_ptr<std::string> clientMessage_;
            std::shared_ptr<bool> isGameReady;
            std::map<udp::endpoint, bool> clients_;
            udp::socket socket_;

            std::string getId(std::string& message);
            std::string getMessage(std::string& message);
        private:
            
            udp::endpoint remote_endpoint_;
            boost::array<char, 1024> recv_buffer_;

            std::mutex mtx;
            
            void start_receive();
            void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
            void update_game_ready();
            void handle_send(boost::shared_ptr<std::string> message,
                            const boost::system::error_code& error,
                            std::size_t bytes) {/* here measure data sent (maybe a .log?)*/};
            std::string binaryToString(const std::string &binaryStr);
            std::string passStringToBinary(const std::string &str);
    };
}