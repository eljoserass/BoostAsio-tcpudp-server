#include "ClientServer.hpp"

using namespace ClientController;

ClientServer::ClientServer(std::string host) {
    available_rooms.push_back(new AvailableRooms(host.c_str(), "1234", "room1", "easy", "4"));
    available_rooms.push_back(new AvailableRooms(host.c_str(), "6969", "room2", "easy", "4"));
}

void ClientServer::startGame(int pos) {
    std::cout << "host " << hostIp << " port " << mainPort;
    udpClient = new UDPClient(available_rooms[pos]->ip, available_rooms[pos]->port);
}

void ClientServer::listen(void) {
    udpClient->receive();
}

void ClientServer::parse_from_cli(void) {
    while (true) {
        std::string message;
        std::cout << "\nEnter command: ";
        std::getline(std::cin, message);

        udpClient->sendCommand(message);
    }
}

void ClientServer::join(void) {
    udpClient->join();
}