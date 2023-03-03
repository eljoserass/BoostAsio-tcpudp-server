
#include "ClientServer.hpp"

using namespace ClientController;

int main(int ac, char **av) {
    NewClientServer *clientServer = new NewClientServer();

    clientServer->startGame(atoi(av[1]));

    clientServer->listen();

    clientServer->parse_from_cli();

    clientServer->join();

    return (0);
}