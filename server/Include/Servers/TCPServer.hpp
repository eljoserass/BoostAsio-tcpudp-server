#pragma once
#include <boost/asio.hpp>
#include "../Room/RoomManager.hpp"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;

class Server {
public:
    Server(boost::asio::io_service &io_service);
    void start();

private:
    void _read(std::vector<std::shared_ptr<tcp::socket>> &clients);
    void _send(const string &message);
    void _parse_commands();

    boost::asio::io_service &_io_service;
    tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<tcp::socket>> _clients;
    RoomManager *_RoomManager;
};
