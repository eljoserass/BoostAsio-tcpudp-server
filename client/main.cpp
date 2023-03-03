#include "ClientServer.hpp"

using namespace ClientController;

void run_receive_thread(udp::socket& socket, udp::endpoint& sender_endpoint) {
    while (true) {
        boost::array<char, 1028> recv_buf;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::cout.write(recv_buf.data(), len);
        std::cout.write("\n", 1);
    }
}

class NewUDPClient {
    public:
        NewUDPClient(std::string ip, std::string port) : 
            io_service(), 
            socket(io_service, udp::endpoint(udp::v4(), 0)), 
            resolver(io_service),
            query(udp::v4(), ip, port),
            sender_endpoint()
        {
           receiver_endpoint =  *resolver.resolve(query);
           sendCommand("clientConnect");
        }

        int sendCommand(std::string command) {
            boost::system::error_code error;
            socket.send_to(boost::asio::buffer(command), receiver_endpoint, 0, error);
            if (error && error != boost::asio::error::message_size) {
                 std::cerr << "Error sending message: " << error.message() << std::endl;
                return (84);
            }
            return (0);
        }

        void receive() {
            receiver_thread = std::thread(run_receive_thread, std::ref(socket), std::ref(sender_endpoint));
        }

        void join() {
            receiver_thread.join();
        }


        boost::asio::io_service io_service;
        udp::socket socket;
        udp::resolver resolver;
        udp::resolver::query query;
        udp::endpoint receiver_endpoint;
        udp::endpoint sender_endpoint;
        std::thread receiver_thread;

};

int main(int ac, char **av) {
    NewUDPClient *client = new NewUDPClient("localhost", av[1]);

    client->receive();
    while (true) {
        std::string message;
        std::cout << "\nEnter command: ";
        std::getline(std::cin, message);

        client->sendCommand(message);
        
    }
    client->join();
    return (0);
}