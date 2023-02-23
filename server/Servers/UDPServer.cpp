#include "../Include/Servers/UDPServer.hpp"

UDPServer::UDPServer(int port) : socket_(io_context, udp::endpoint(udp::v4(), port)), mtx(), io_context()
{
    start_receive();
    clientMessage_ = std::make_shared<std::string>(std::string("default message"));
}

void UDPServer::send_to_all(const std::string &message)
{
    for (const auto &client : clients_) {
        // if (client.second == true) {
            udp::endpoint endpoint = client.first;
            socket_.send_to(boost::asio::buffer(message), endpoint);
        // }
    }
}

void UDPServer::run(void)
{
    io_context.run();
}

// void UDPServer::remove_players() {
//     std::map<udp::endpoint, bool>::iterator it
//     for (const auto &client : clients_) {
//         clients_.put(client.first, false)
//     }
// }

void UDPServer::start_receive() {
    std::cout << "start_recieve" << std::endl;
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&UDPServer::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void UDPServer::handle_receive(const boost::system::error_code& error, std::size_t received) {
    std::cout << "handle_receive" << std::endl;
    if (!error) {
        if (clients_.count(remote_endpoint_) == 0) {
            clients_[remote_endpoint_] = true;
        }
        boost::shared_ptr<std::string> message(
                new std::string("server say hello"));
        mtx.lock(); 
        std::cout << "received in server from client "<<  std::string(recv_buffer_.begin(), received)<< std::endl;
        clientMessage_ = std::make_shared<std::string>(std::string(recv_buffer_.begin(), received));
        mtx.unlock();
        socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
            boost::bind(&UDPServer::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

        start_receive();
    }
}