#include "UDPServer.hpp"
#include "GameManager.hpp"
using namespace Server;

class ECSinmain: public AbstractECS {
    public:
        ECSinmain(){}

        void logic(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady,  std::map<udp::endpoint, bool>& players, udp::socket& sender) {
            sleep(1);
            *ECSResponse = " THIS IS SENT FROM ECS (IN MAIN) TO CLIENT " + std::to_string(rand());
            
            std::cout << "hello from ecs IN MAIN, this is what the client said: " << clientMessage->c_str() << std::endl;
            std::cout << "ECS (in main) says to client " << *ECSResponse << std::endl;
        }
};

class RTYPE: public AbstractECS {
    public:
        RTYPE(){}

        void logic(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady,  std::map<udp::endpoint, bool>& players, udp::socket& sender) {
            sleep(1);
            *ECSResponse = " THIS IS SENT FROM ECS (IN MAIN) TO CLIENT " + std::to_string(rand());
            
            std::cout << "hello from ecs IN MAIN, this is what the client said: " << clientMessage->c_str() << std::endl;
            std::cout << "ECS (in main) says to client " << *ECSResponse << std::endl;
        }
};

class MINESWEEPER: public AbstractECS {
    public:
        MINESWEEPER(){}

        void logic(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady,  std::map<udp::endpoint, bool>& players, udp::socket& sender) {
            sleep(1);
            *ECSResponse = " THIS IS SENT FROM ECS (IN MAIN) TO CLIENT " + std::to_string(rand());
            
            std::cout << "hello from ecs IN MAIN, this is what the client said: " << clientMessage->c_str() << std::endl;
            std::cout << "ECS (in main) says to client " << *ECSResponse << std::endl;

        }
};


int main()
{
    boost::asio::io_context io_context;

    ECSinmain childObj;
    AbstractECS* ptr;
    ptr = &childObj;

    GameManager *gamemaanager = new GameManager();

    gamemaanager->startGame("room1", 1234, io_context, ptr);
    gamemaanager->startGame("room2", 6969, io_context, ptr);

    io_context.run();

    gamemaanager->joinThreads();

    return 0;
}