#pragma once

#include "../Servers/UDPServer.hpp"

/*
Class that contains all info about communicating the game state provided by the ECS(Engine)
to the players and viceversa, passing info from the clients to the ECS
*/

class ECS {
    public:
        ECS(){

        }
        void run(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse) {
            while (true) {
                sleep(4);
                std::cout << "hello from ecs, this is what the client said: " << clientMessage->c_str() << std::endl;
                *ECSResponse = " THIS IS SENT FROM ECS TO CLIENT ";
                std::cout << "ECS says to client " << *ECSResponse << std::endl;
            }
        }
};

namespace Server {
    class Game {
        public:
            Game(int port, boost::asio::io_context &io_context);
            void run(void);
        private:
            void run_ecs(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse);
            int port_;
            bool isRunning;
            ECS ecs_;
            UDPServer server_;
            std::shared_ptr<std::string> ECSResponse_;
            std::thread server_thread;
            std::thread sender_thread;
    };
}