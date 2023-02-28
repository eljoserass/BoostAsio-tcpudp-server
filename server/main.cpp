#include "TCPServer.hpp"
using namespace Server;

int main()
{
    boost::asio::io_service io_service;

    TCPServer server(io_service);
    server.start();

    return 0;
}