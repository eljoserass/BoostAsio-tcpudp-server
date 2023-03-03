#include "UDPClient.hpp"

using namespace ClientController;

void run_receive_thread(udp::socket& socket, udp::endpoint& sender_endpoint, std::shared_ptr<std::string>& response) {
    while (true) {
        boost::array<char, 1028> recv_buf;
        
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        *response = std::string(recv_buf.data(), len);

        /// REMOVE WHEN NOT USING CLI!
        std::cout.write(recv_buf.data(), len);
        std::cout.write("\n", 1);
    }
}

UDPClient::UDPClient(std::string ip, std::string port) : 
    io_service(), 
    socket(io_service, udp::endpoint(udp::v4(), 0)), 
    resolver(io_service),
    query(udp::v4(), ip, port),
    sender_endpoint()
{
    response = std::make_shared<std::string>(std::string("null"));
    receiver_endpoint =  *resolver.resolve(query);
    sendCommand("clientConnect");
}

std::string UDPClient::passToBinary(std::string command)
{
    std::stringstream buffer;
    msgpack::pack(buffer, command);
    std::string result;
    std::string binary = buffer.str();

    for (int i = 1; i < binary.size(); i++) {
        char hex[3];
        sprintf(hex, "%02x", (unsigned char)binary[i]);
        result.append(hex);
    }
    return result;
}

int UDPClient::sendCommand(std::string command) {
    std::string binary = passToBinary(command);
    boost::system::error_code error;
    socket.send_to(boost::asio::buffer(binary), receiver_endpoint, 0, error);
    if (error && error != boost::asio::error::message_size) {
        std::cerr << "Error sending message: " << error.message() << std::endl;
        return (84);
    }
    return (0);
}

void UDPClient::receive() {
    receiver_thread = std::thread(run_receive_thread, std::ref(socket), std::ref(sender_endpoint), std::ref(response));
}

void UDPClient::join() {
    receiver_thread.join();
}