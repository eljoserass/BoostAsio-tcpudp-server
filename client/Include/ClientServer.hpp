#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;


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

namespace ClientController {

class NewUDPClient {
    public:
        NewUDPClient(std::string ip, std::string port) : 
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
            receiver_thread = std::thread(run_receive_thread, std::ref(socket), std::ref(sender_endpoint), std::ref(response));
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
        std::shared_ptr<std::string> response;

};

class AvailableRooms {
    public:

        AvailableRooms(const char* ipParam, const char* portParam, const char* nameParam, const char* difficultyParam, const char *maxPlayersParam) : 
        ip(ipParam), port(portParam), name(nameParam), difficulty(difficultyParam), max_players(maxPlayersParam) {
        }

        std::string ip;
        std::string port;
        std::string name;
        std::string difficulty;
        std::string max_players;
};

class NewClientServer {
    public:
        NewClientServer();

        void startGame(int pos);

        void listen(void) ;

        void parse_from_cli(void) ;
        
        void join(void);
        NewUDPClient *udpClient;
        std::vector<AvailableRooms*> available_rooms;
};

}