#include "../Include/Game/GameManager.hpp"

void run_game_thread(Game *game)
{
    game->run();
}

int getAvailablePort() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = 0;

    if (bind(sock, (sockaddr *)&address, sizeof(address)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    socklen_t addressLength = sizeof(address);
    if (getsockname(sock, (sockaddr *)&address, &addressLength) == -1) {
        std::cerr << "Error getting socket name" << std::endl;
        return -1;
    }

    int port = ntohs(address.sin_port);
    close(sock);
    return port;
}

int GameManager::startGame(std::string room)
{
    int port = -1;

    if (games_.count(room.c_str()) == 0) {
        port = getAvailablePort();
        std::cout << "game started in port " << port << std::endl;
        games_[room.c_str()] = new Game(port);
        Game *tempgame = games_[room.c_str()];
        gamesTread.push_back(std::thread(run_game_thread, tempgame));
    }
    return port;
}
void GameManager::endGameByRoomId(std::string room) {
    games_.erase(room.c_str());
}
void GameManager::joinThreads(void) {
    for (int i = 0; i < gamesTread.size(); i++) {
        gamesTread[i].join();
    }
}