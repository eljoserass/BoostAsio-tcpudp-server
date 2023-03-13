
#include "ClientServer.hpp"

using namespace ClientController;

int main(int ac, char **av) {

    if (ac < 3) {
        return 84;
    }
    if (av[1] == "-h" || av[1] == "--help") {
        std::cout << "FIRST ARG = IP OF SERVER" << std::endl;
        std::cout << "SECOND ARG = ROOM" << std::endl;
        return (0);
    }
    ClientServer *clientServer = new ClientServer() ;
    std::cout << "av1: " << av[1] << " av2  " << av[2] << std::endl;
    std::string *ip = new std::string(av[1]);
    std::string *port = new std::string(av[2]);

    std::cout << *ip << std::endl;
    clientServer->setIp(*ip);
    clientServer->setPort(*port);

    clientServer->startGame();

    clientServer->listen();

    clientServer->parse_from_cli();

    clientServer->join();

    return (0);
}