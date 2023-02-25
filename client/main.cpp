#include "ClientServer.hpp"

int main(int argc, char **argv) {
    try {

        // create client controller
        ClientServer *clientServer = new ClientServer();
        // set host
        clientServer->setHost("127.0.0.1");
        // set tcp connection
        clientServer->setTcpPort("9999");
        // set name of player
        clientServer->setPlayerName("name");

        // start the connection
        clientServer->run();

        // create a room
        clientServer->createRoom("rowwwom");

        // read avaialbe rooms to join
        std::vector<std::string> rooms = clientServer->clientData->currentAvailableRooms;

        // get players in a room 
        std::vector<std::tuple<std::string, bool>> playes_in_room = clientServer->clientData->currentRoomPlayers;

        // get out of room
        clientServer->leaveRoom();

        // set a player is ready to play or not
        clientServer->setReady(true);

        // the connection was succesful when is true
        bool isConnected = clientServer->clientData->isConnected;

        // when a player enters in a room this is set to true
        bool isInRoom = clientServer->clientData->isInRoom;

        // when all the players setReady(true), this will be true and have to change to the actual game screen
        bool isInGame = clientServer->clientData->isInGame;

        // call it after your while(isWindowOpen)
        clientServer->joinThread();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}