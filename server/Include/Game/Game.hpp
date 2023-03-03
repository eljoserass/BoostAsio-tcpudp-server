#pragma once

#include "../Server/UDPServer.hpp"

/*
Class that contains all info about communicating the game state provided by the ECS(Engine)
to the players and viceversa, passing info from the clients to the ECS
*/

class AbstractECS {
    public:
        virtual void logic(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameRead) = 0;
       
        void block(std::shared_ptr<bool>& isGameRead) {
            while (!*isGameRead) {}
        }

        virtual void run(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameRead) {
            block(isGameRead);
            while(true)
                logic(clientMessage, ECSResponse, isGameRead);
        }
};

namespace Server {
    class Game {
        public:
            Game(int port, boost::asio::io_context &io_context, AbstractECS *ecs);
            void run(void);
        private:
            void run_ecs(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse, std::shared_ptr<bool>& isGameReady);
            int port_;
            bool isRunning;
            AbstractECS *_ecs;
            UDPServer server_;
            std::shared_ptr<std::string> ECSResponse_;
            std::thread server_thread;
            std::thread sender_thread;
    };
}