#include "TCPServer.hpp"
using namespace Server;

int main()
{
    boost::asio::io_service io_service;

    AsyncTcpServer server(io_service, 9999);

    io_service.run();

    return 0;
}