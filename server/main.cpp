#include "TCPServer.hpp"
using namespace Server;

int main()
{
    // boost::asio::io_service io_service;
    boost::asio::io_context io_context;

    GameManager *gamemaanager = new GameManager();

    gamemaanager->startGame("room1", 1234, io_context);
    gamemaanager->startGame("room2", 6969, io_context);

    io_context.run();

    gamemaanager->joinThreads();

    // io_service.run();

    return 0;
}