#pragma once

#include "../Server/UDPServer.hpp"

/*
Class that contains all info about communicating the game state provided by the ECS(Engine)
to the players and viceversa, passing info from the clients to the ECS
*/

class AbstractECS {
    public:
        virtual void logic(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady, std::map<udp::endpoint, bool>& players, udp::socket& socket) = 0;
       
        void block(std::shared_ptr<bool>& isGameRead) {
            while (!*isGameRead) {}
        }
        std::string getId(std::string& message) {
            std::vector <std::string> parsed;
            boost::split(parsed, message, boost::is_any_of("/"));
            return (parsed[0]);
        }

        std::string getMessage(std::string& message) {
            std::vector <std::string> parsed;
            boost::split(parsed, message, boost::is_any_of("/"));
            return (parsed[1]);
        }


        void send_to_client(udp::endpoint& client_endpoint, udp::socket& sender,std::string& message) {
            sender.send_to(boost::asio::buffer(message), client_endpoint);
        }

        virtual void run(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady, std::map<udp::endpoint, bool>& players, udp::socket& sender) {
            block(isGameReady);
            while(true)
                logic(clientMessage, ECSResponse, isGameReady, players, sender);
        }
};


namespace Server {
    class Game {
        public:
            Game(int port, boost::asio::io_context &io_context, AbstractECS *ecs);
            void run(void);
        private:
            void run_ecs(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse, std::shared_ptr<bool>& isGameReady, std::map<udp::endpoint, bool>& players, udp::socket &socket_);
            int port_;
            bool isRunning;
            AbstractECS *_ecs;
            UDPServer server_;
            std::shared_ptr<std::string> ECSResponse_;
            std::thread server_thread;
            std::thread sender_thread;
    };
}