#include "ClientServer.hpp"

using namespace ClientController;

// int main(int argc, char **argv) {
//     try {

//         // create client controller
//         ClientServer *clientServer = new ClientServer();

//         clientServer->startUdp(argv[1]);
//         //clientServer->startUdp("6969");

//         clientServer->sendUdpCommand("awawawawawwa in the port " + std::string(argv[1]));

//         // std::string input;
//         // while (true) {
//         //     std::cout << "before getline" << *clientServer->gameState << std::endl;
//         //     std::getline (std::cin,input);
//         //     clientServer->sendUdpCommand(input);

//         // }

//         clientServer->joinThread();
//         // set host
//         // clientServer->setHost("127.0.0.1");
//         // // set tcp connection
//         // clientServer->setTcpPort("9999");
//         // // set name of player

//         // // start the connection
//         // clientServer->run();

//         // // sleep(1);
//         // clientServer->setPlayerName("name" + std::string(argv[1]) );
//         // // sleep(1);
//         // // create a room
//         // std::cout << "CREATE  ROOM" << std::endl;
//         // clientServer->createRoom("room " + std::string(argv[2]));
//         // sleep(1);
//         // clientServer->fetchRooms();

//         // sleep(1);

//         // clientServer->fetchRooms();

//         // for (auto& room: clientServer->clientData->currentAvailableRooms) {
//         //         std::cout << "["<< std::get<0>(room) << "]"<< std::endl;
//         //     }

//         // std::cout << "uuid" << clientServer->findRoomUuidByName("room " + std::string(argv[2]));

//         // return (0);
//         // clientServer->leaveRoom();

//         // clientServer->enterRoom("room " + std::string(argv[2]));

//         // clientServer->setReady(true);

//         // clientServer->isCurrentRoomReady();

//         // if (clientServer->clientData->isInGame) {
//         //     std::cout << "GAME STARTED!" << std::endl;
//         // }

//         // // read avaialbe rooms to join
//         // std::vector<std::tuple<std::string, std::string>> rooms = clientServer->clientData->currentAvailableRooms;

//         // // get players in a room 
//         // std::vector<std::string> playes_in_room = clientServer->clientData->currentRoomPlayersName;

//         // // get out of room
//         // clientServer->leaveRoom();

//         // // set a player is ready to play or not
//         // clientServer->setReady(true);

//         // // the connection was succesful when is true
//         // bool isConnected = clientServer->clientData->isConnected;

//         // // when a player enters in a room this is set to true
//         // bool isInRoom = clientServer->clientData->isInRoom;

//         // // when all the players setReady(true), this will be true and have to change to the actual game screen
//         // bool isInGame = clientServer->clientData->isInGame;

//         // // call it after your while(isWindowOpen)
//         // clientServer->joinThread();

//         // while (true) {
//         //     sleep(1);
//         //     clientServer->fetchRooms();
//         //     std::cout << "playerID " << clientServer->clientData->clientId << std::endl;
//         //     // clientServer->clientData->currentAvailableRooms.push_back(std::make_tuple("hola1", "hola2"));
//         //     std::cout << "rooms available:" << std::endl;
//         //     for (auto& room: clientServer->clientData->currentAvailableRooms) {
//         //         std::cout << "["<< std::get<0>(room) << "]"<< std::endl;
//         //     }
//         //     // std::cout << "uuid " << clientServer->findRoomUuidByName("room " + std::string(argv[2]));
//         //     clientServer->enterRoom("room " + std::string(argv[2]));
//         //     sleep(1);
//         //     clientServer->setReady(true);
//             // clientServer->deleteRoomByName("room " + std::string(argv[2]));
//             // std::cout << "CURRENT ROOM ID  " << clientServer->clientData->currentRoomID; 
//             // clientServer->fetchPlayersInRoom();
//         // }

//     } catch (std::exception& e) {
//         std::cerr << e.what() << std::endl;
//     }
//     return (0);
// }



int main(int ac, char **av) {
  boost::asio::io_service io_service;
  udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));

  udp::resolver resolver(io_service);
  udp::resolver::query query(udp::v4(), "localhost", av[1]);
  udp::endpoint receiver_endpoint = *resolver.resolve(query);
  udp::endpoint sender_endpoint;

  while (true) {
    std::string message;
    std::cout << "Enter command: ";
    std::getline(std::cin, message);

    boost::system::error_code error;
    socket.send_to(boost::asio::buffer(message), receiver_endpoint, 0, error);
    boost::array<char, 128> recv_buf;
    size_t len = socket.receive_from(
    boost::asio::buffer(recv_buf), sender_endpoint);
    std::cout.write(recv_buf.data(), len);
    if (error && error != boost::asio::error::message_size) {
      std::cerr << "Error sending message: " << error.message() << std::endl;
      break;
    }
  }

  return 0;
}


// void receive_messages(udp::socket& socket) {
//   while (true) {
//     boost::array<char, 1024> recv_buffer;
//     udp::endpoint sender_endpoint;
//     boost::system::error_code error;
//     size_t length = socket.receive_from(
//         boost::asio::buffer(recv_buffer), sender_endpoint, 0, error);

//     if (error && error != boost::asio::error::message_size) {
//       std::cerr << "Error receiving message: " << error.message() << std::endl;
//       break;
//     }

//     std::cout << "Received message: ";
//     std::cout.write(recv_buffer.data(), length);
//     std::cout << std::endl;
//   }
// }

// int main() {
//   boost::asio::io_service io_service;
//   udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));

//   udp::resolver resolver(io_service);
//   udp::resolver::query query(udp::v4(), "localhost", "9876");
//   udp::endpoint receiver_endpoint = *resolver.resolve(query);

//   std::thread receive_thread([&socket] { receive_messages(socket); });

//   while (true) {
//     std::string message;
//     std::cout << "Enter command: ";
//     std::getline(std::cin, message);

//     boost::system::error_code error;
//     socket.send_to(boost::asio::buffer(message), receiver_endpoint, 0, error);

//     if (error && error != boost::asio::error::message_size) {
//       std::cerr << "Error sending message: " << error.message() << std::endl;
//       break;
//     }
//   }

//   receive_thread.join();

//   return 0;
// }
