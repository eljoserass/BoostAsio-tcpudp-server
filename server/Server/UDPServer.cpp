#include "../Include/Server/UDPServer.hpp"
using namespace Server;

UDPServer::UDPServer(int port, boost::asio::io_context &io_context) : socket_(io_context, udp::endpoint(udp::v4(), port)), mtx()
{
    start_receive();
    clientMessage_ = std::make_shared<std::string>(std::string("default message"));
    isGameReady = std::make_shared<bool>(bool(false));
}

std::string UDPServer::getId(std::string& message) {
    std::vector <std::string> parsed;
    boost::split(parsed, message, boost::is_any_of("/"));
    return (parsed[0]);
}

std::string UDPServer::getMessage(std::string& message) {
    std::vector <std::string> parsed;
    boost::split(parsed, message, boost::is_any_of("/"));
    return (parsed[1]);
}

std::string UDPServer::passStringToBinary(const std::string &str)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(str.c_str());
    std::string result;
    for (std::size_t i = 0; i < str.size(); ++i) {
        std::bitset<8> byte(data[i]);
        result += byte.to_string();
    }
    return result;
}

void UDPServer::send_to_all(const std::string& message) {
    if (*isGameReady) {
        std::string binaryMessage = passStringToBinary(message);
        for (const auto& client : clients_) {
            if (client.second == true) {
                udp::endpoint endpoint = client.first;
                socket_.async_send_to(boost::asio::buffer(binaryMessage), remote_endpoint_,
                    boost::bind(&UDPServer::handle_send, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

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

std::string UDPServer::binaryToString(const std::string &binaryStr)
{
    std::string result;
    std::bitset<8> bits;
    for (size_t i = 0; i < binaryStr.size(); i += 8) {
        bits = std::bitset<8>(binaryStr.substr(i, 8));
        char c = static_cast<char>(bits.to_ulong());
        result.push_back(c);
    }
    return result;
}

void UDPServer::handle_receive(const boost::system::error_code& error, std::size_t received) {
    if (!error) {
        if (clients_.count(remote_endpoint_) == 0) {
            clients_[remote_endpoint_] = false;
        }
        std::cout << "RECEIVED FROM  " << remote_endpoint_ << std::endl;
        boost::shared_ptr<std::string> message(new std::string("ok"));
        std::stringstream buffer_message;

        if (recv_buffer_.begin() != nullptr) {
            std::string received_string = binaryToString(std::string(recv_buffer_.begin(), received));
            buffer_message << remote_endpoint_ << "/" << received_string; 
        } else {
            std::cerr << "Error: recv_buffer_.begin() is null" << std::endl;
            return;
        }

        mtx.lock();
        std::cout << "received in server from client " << buffer_message.str() << std::endl;
        clientMessage_ = std::make_shared<std::string>(buffer_message.str());
        mtx.unlock();

        std::map<udp::endpoint, bool>::iterator it = clients_.find(remote_endpoint_); 
        if (getMessage(*clientMessage_) == "ready") {
            if (it != clients_.end())
                it->second = true;
        } else if (getMessage(*clientMessage_) == "notready"){
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
