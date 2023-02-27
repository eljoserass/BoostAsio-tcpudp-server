#include "ClientServer.hpp"

using namespace ClientController;

UDPClient::UDPClient(std::string host, std::string port, std::shared_ptr<std::string>& gameState): resolver_(io_context_), socket_(io_context_)
{
    receiver_endpoint_ = *resolver_.resolve(udp::v4(), host.c_str(), port.c_str()).begin();
    socket_.open(udp::v4());
}

void listen_from_UDPserver(udp::socket& socket, std::shared_ptr<std::string>& gameState, std::shared_ptr<bool> &running)
{
    boost::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    while (*running) {
        size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);
        *gameState = std::string(recv_buf.begin(), len);
        std::cout << " received from server: "<< std::string(recv_buf.begin(), len) <<std::endl;
    }
}

void UDPClient::run(std::shared_ptr<std::string>& gameState, std::shared_ptr<bool> running)
{
    listen_thread_ = std::thread(listen_from_UDPserver, std::ref(socket_), std::ref(gameState), std::ref(running));;
}

void UDPClient::joinThread()
{
    listen_thread_.join();
}
