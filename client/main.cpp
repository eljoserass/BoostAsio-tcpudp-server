
#include "ClientServer.hpp"

using namespace ClientController;

int main(int ac, char **av) {
    ClientServer *clientServer = new ClientServer();

    clientServer->startGame(atoi(av[1]));

    clientServer->listen();

    clientServer->parse_from_cli();
    // clientServer->close();

    clientServer->join();

    return (0);
}