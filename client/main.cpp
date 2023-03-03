#include "ClientServer.hpp"

using namespace ClientController;

int main(int argc, char **argv) {
    try {

        // create client controller
        ClientServer *clientServer = new ClientServer();

        clientServer->startUdp("1234");
        //clientServer->startUdp("6969");

        clientServer->joinThread();

        while (true) {
            std::cout << "before getline" << *clientServer->gameState << std::endl;

        }
        // set host
        // clientServer->setHost("127.0.0.1");
        // // set tcp connection
        // clientServer->setTcpPort("9999");
        // // set name of player

        // // start the connection
        // clientServer->run();

        // // sleep(1);
        // clientServer->setPlayerName("name" + std::string(argv[1]) );
        // // sleep(1);
        // // create a room
        // std::cout << "CREATE  ROOM" << std::endl;
        // clientServer->createRoom("room " + std::string(argv[2]));
        // sleep(1);
        // clientServer->fetchRooms();

        // sleep(1);

        // clientServer->fetchRooms();

        // for (auto& room: clientServer->clientData->currentAvailableRooms) {
        //         std::cout << "["<< std::get<0>(room) << "]"<< std::endl;
        //     }

        // std::cout << "uuid" << clientServer->findRoomUuidByName("room " + std::string(argv[2]));

        // return (0);
        // clientServer->leaveRoom();

        // clientServer->enterRoom("room " + std::string(argv[2]));

        // clientServer->setReady(true);

        // clientServer->isCurrentRoomReady();

        // if (clientServer->clientData->isInGame) {
        //     std::cout << "GAME STARTED!" << std::endl;
        // }

        // // read avaialbe rooms to join
        // std::vector<std::tuple<std::string, std::string>> rooms = clientServer->clientData->currentAvailableRooms;

        // // get players in a room 
        // std::vector<std::string> playes_in_room = clientServer->clientData->currentRoomPlayersName;

        // // get out of room
        // clientServer->leaveRoom();

        // // set a player is ready to play or not
        // clientServer->setReady(true);

        // // the connection was succesful when is true
        // bool isConnected = clientServer->clientData->isConnected;

        // // when a player enters in a room this is set to true
        // bool isInRoom = clientServer->clientData->isInRoom;

        // // when all the players setReady(true), this will be true and have to change to the actual game screen
        // bool isInGame = clientServer->clientData->isInGame;

        // // call it after your while(isWindowOpen)
        // clientServer->joinThread();

        while (true) {
            sleep(1);
            clientServer->fetchRooms();
            std::cout << "playerID " << clientServer->clientData->clientId << std::endl;
            // clientServer->clientData->currentAvailableRooms.push_back(std::make_tuple("hola1", "hola2"));
            std::cout << "rooms available:" << std::endl;
            for (auto& room: clientServer->clientData->currentAvailableRooms) {
                std::cout << "["<< std::get<0>(room) << "]"<< std::endl;
            }
            // std::cout << "uuid " << clientServer->findRoomUuidByName("room " + std::string(argv[2]));
            clientServer->enterRoom("room " + std::string(argv[2]));
            sleep(1);
            clientServer->setReady(true);
            // clientServer->deleteRoomByName("room " + std::string(argv[2]));
            // std::cout << "CURRENT ROOM ID  " << clientServer->clientData->currentRoomID; 
            // clientServer->fetchPlayersInRoom();
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}