#include "ClientServer.hpp"

using namespace ClientController;




NewClientServer::NewClientServer() {
    available_rooms.push_back(new AvailableRooms("localhost", "1234", "room1", "easy", "4"));
    available_rooms.push_back(new AvailableRooms("localhost", "6969", "room2", "easy", "4"));
}

void NewClientServer::startGame(int pos) {
    udpClient = new NewUDPClient(available_rooms[0]->ip, available_rooms[0]->port);
}

void NewClientServer::listen(void) {
    udpClient->receive();
}

void NewClientServer::parse_from_cli(void) {
    while (true) {
        std::string message;
        std::cout << "\nEnter command: ";
        std::getline(std::cin, message);

        udpClient->sendCommand(message);
    }
}

void NewClientServer::join(void) {
    udpClient->join();
}