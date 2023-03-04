#include "../Include/Server/UDPServer.hpp"
using namespace Server;

UDPServer::UDPServer(int port, boost::asio::io_context &io_context) : socket_(io_context, udp::endpoint(udp::v4(), port)), mtx()
{
    start_receive();
    clientMessage_ = std::make_shared<std::string>(std::string("default message"));
    isGameReady = std::make_shared<bool>(bool(false));
}

void UDPServer::send_to_all(const std::string &message)
{
    if (*isGameReady) {
        for (const auto &client : clients_) {
            if (client.second == true) {
                udp::endpoint endpoint = client.first;
                socket_.send_to(boost::asio::buffer(message), endpoint);
            }
        }
    }  
}

void UDPServer::start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&UDPServer::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void UDPServer::update_game_ready() {
    int count_ready = 0;
    for (auto& client: clients_) {
        if (client.second)
            count_ready++;
    }
    if (count_ready >= clients_.size()) {
        *isGameReady = true;
    }
}

void UDPServer::handle_receive(const boost::system::error_code& error, std::size_t received) {
    if (!error) {
        if (clients_.count(remote_endpoint_) == 0) {
            clients_[remote_endpoint_] = false;
        }
        boost::shared_ptr<std::string> message(
                new std::string("ok"));
        mtx.lock();
        std::cout << "received in server from client "<<  std::string(recv_buffer_.begin(), received) << std::endl;
        clientMessage_ = std::make_shared<std::string>(std::string(recv_buffer_.begin(), received));
        mtx.unlock();

        std::map<udp::endpoint, bool>::iterator it = clients_.find(remote_endpoint_); 
        if (*clientMessage_ == "ready") {
            if (it != clients_.end())
                it->second = true;
        } else if (*clientMessage_ == "notready"){
            if (it != clients_.end())
                it->second = false;
        }
        update_game_ready();
        socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
            boost::bind(&UDPServer::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

        start_receive();
    }
}