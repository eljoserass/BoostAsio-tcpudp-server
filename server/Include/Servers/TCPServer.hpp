#pragma once
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include "Room/RoomManager.hpp"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;

namespace Server {
    class TCPServer {
        public:
            TCPServer(boost::asio::io_service &io_service);
            void start();

            vector<string> _players;

        private:
            void _read(std::vector<std::shared_ptr<tcp::socket>> &clients);
            void _sendToClients(const string &message);
            void _sendToClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string &message);
            void handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket);

            boost::asio::io_service &_io_service;
            tcp::acceptor _acceptor;
            std::vector<std::shared_ptr<tcp::socket>> _clients;
            RoomManager *_RoomManager;
    };
}
