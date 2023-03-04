#include "../Include/Game/Game.hpp"
using namespace Server;

void send_response(UDPServer &server, std::shared_ptr<std::string> &ECSResponse) 
{
    while (true) {
        sleep(1);
        server.send_to_all(ECSResponse->c_str());
    }
}

Game::Game(int port, boost::asio::io_context &io_context, AbstractECS *ecs) : server_(port, io_context)
{
    ECSResponse_ = std::make_shared<std::string>(std::string(" default message ecs"));
    sender_thread = std::thread(send_response, std::ref(server_), std::ref(ECSResponse_));
    port_ = port;
    isRunning = true;
    _ecs = ecs;
}

void Game::run()
{  
    run_ecs(server_.clientMessage_, ECSResponse_ , server_.isGameReady, server_.clients_, server_.socket_);
    // server_thread.join();
    sender_thread.join();
    isRunning = false;
}

void Game::run_ecs(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse,  std::shared_ptr<bool>& isGameReady, std::map<udp::endpoint, bool>& players, udp::socket &socket)
{
    _ecs->run(clientMessage,ECSResponse, isGameReady, players, socket);
}