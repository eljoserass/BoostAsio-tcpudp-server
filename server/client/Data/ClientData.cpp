#include "ClientData.hpp"

ClientData::ClientData()
{
    currentRoomName = std::string("");
    currentRoomID = std::string("");
    currentRoomPlayers = std::vector<std::string>(1);
    currentRoomPlayers[0] = std::string("");
    currentAvailableRooms = std::vector<std::string>(1);
    currentAvailableRooms[0] = std::string("");
    currentGamePort = std::string("");
    clientId = std::string("");
    clientName = std::string("");
    isInGame = false;
}