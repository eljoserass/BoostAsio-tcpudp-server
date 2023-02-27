#include "../Include/Game/Game.hpp"
using namespace Server;

void run_server(UDPServer &server)
{
    server.run();
}

void send_response(UDPServer &server, std::shared_ptr<std::string> &ECSResponse) 
{
    while (true) {
        sleep(1);
        server.send_to_all(ECSResponse->c_str());
    }
}

Game::Game(int port) : server_(port), ecs_()
{
    ECSResponse_ = std::make_shared<std::string>(std::string(" default message ecs"));
    server_thread = std::thread(run_server, std::ref(server_));
    sender_thread = std::thread(send_response, std::ref(server_), std::ref(ECSResponse_));
    port_ = port;
    isRunning = true;
}

void Game::run()
{  
    run_ecs(server_.clientMessage_, ECSResponse_);
    server_thread.join();
    sender_thread.join();
    isRunning = false;
}

void Game::run_ecs(std::shared_ptr<std::string> &clientMessage, std::shared_ptr<std::string>& ECSResponse)
{
    ecs_.run(clientMessage,ECSResponse);
}