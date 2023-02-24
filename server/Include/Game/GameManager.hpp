#pragma once

#include <iostream>
#include <vector>

#include "Game/Game.hpp"
#include "Room/Room.hpp"

/*
Manager in between the Rooms and The Games with ECS and UDP sockets
The mainServer will use this to manager to orchestrate what happens with the game logic
*/

class GameManager {
    public:
        int startGame(std::string room);
        void endGameByRoomId(std::string room);
        void joinThreads(void);
    private:
        std::map<std::string, Game*> games_;
        std::vector<std::thread> gamesTread;
};