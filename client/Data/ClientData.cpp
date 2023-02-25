#include "ClientData.hpp"

ClientData::ClientData()
{
    currentRoomName = std::string("");
    currentRoomID = std::string("");
    currentRoomPlayers.push_back(std::make_tuple("player1", false));
    currentRoomPlayers.push_back(std::make_tuple("player2", false));
    currentRoomPlayers.push_back(std::make_tuple("player3", false));
    currentRoomPlayers.push_back(std::make_tuple("player4", false));
    currentAvailableRooms.push_back("room1");
    currentAvailableRooms.push_back("room2");
    currentAvailableRooms.push_back("room3");
    currentAvailableRooms.push_back("room4");
    currentGamePort = std::string("");
    clientId = std::string("");
    clientName = std::string("");
    isInGame = false;
    isReady = false;
    isInRoom = false;
    isConnected = false;
}