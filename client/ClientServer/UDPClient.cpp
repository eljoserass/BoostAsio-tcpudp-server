#include "UDPClient.hpp"

using namespace ClientController;

std::string readStringFromBinary(std::istringstream &iss) {
    std::ostringstream oss;
    std::copy(std::istreambuf_iterator<char>(iss),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(oss));
    return oss.str();
}

void run_receive_thread(udp::socket& socket, udp::endpoint& sender_endpoint, std::shared_ptr<std::string>& response) {
    while (true) {
        boost::array<char, 1028> recv_buf;

        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::istringstream iss(std::string(recv_buf.data(), len));
        std::string strData = readStringFromBinary(iss);

        *response = strData;

        /// REMOVE WHEN NOT USING CLI!
        // std::cout.write(recv_buf.data(), len);
        // std::cout.write("\n", 1);
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

std::string UDPClient::passStringToBinary(const std::string &str)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(str.c_str());
    std::string result;
    for (std::size_t i = 0; i < str.size(); ++i) {
        std::bitset<8> byte(data[i]);
        result += byte.to_string();
    }
    return result;
}

int UDPClient::sendCommand(std::string command) {
    boost::system::error_code error;
    boost::system::error_code ec;
    boost::asio::ip::udp::endpoint endpoint = socket.remote_endpoint(ec);
    std::string binaryData = passStringToBinary(command);

    socket.send_to(boost::asio::buffer(binaryData), receiver_endpoint, 0, error);
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