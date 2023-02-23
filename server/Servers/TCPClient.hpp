#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

class Client
{
    public:
        Client(boost::asio::io_service &io_service, tcp::socket &socket);
        void start();

    private:
        boost::asio::io_service &_io_service;
        tcp::socket &_socket;
        bool _isConnected;

        void _send(const string &message);
        void _receive();
};
