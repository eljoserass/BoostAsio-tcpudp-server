#pragma once
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "RoomManager.hpp"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;

namespace Server {
    class TCPServer {
        public:
            TCPServer(boost::asio::io_service &io_service);
            void start();

            vector<string> _players;

            void _parse_commands();
            void _read();
            void _sendToClients(const string &message);
            void _sendToClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string &message);
            void handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
            // void start_accept(void) {
            //     tcp_connection::pointer new_connection =
            //     tcp_connection::create(io_context_);

            //     acceptor_.async_accept(new_connection->socket(),
            //         boost::bind(&tcp_server::handle_accept, this, new_connection,
            //         boost::asio::placeholders::error));
            // }

            boost::asio::io_service &_io_service;
            tcp::acceptor _acceptor;
            
            std::vector<std::shared_ptr<tcp::socket>> _clients;
            RoomManager *_RoomManager;
            std::thread read_thread;
    };

    class AsyncTcpServer {
        public:
            AsyncTcpServer(io_service& io_service, int port)
                : ioService(io_service), m_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
            {
                _RoomManager = new RoomManager();
                startAccept();
            }

            void callFunctions(std::string command, std::string param1, std::string param2)
            {
                if (command == "create_room") {
                    cout << "room created\n";
                    _RoomManager->createRoom(param1);
                }
                if (command == "delete_room") {
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid roomUuid = gen(param1);
                    _RoomManager->deleteRoomById(roomUuid);
                }
                if (command == "add_player_room") {
                    // e.g add_player_room;roomId:playerId
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid roomUuid = gen(param1);
                    boost::uuids::uuid playerUuid = gen(param2);
                    _RoomManager->addPlayerToRoom(roomUuid, playerUuid);
                }
                if (command == "remove_player_room") {
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid playerUuid = gen(param1);
                    _RoomManager->removePlayerFromRoom(playerUuid);
                }
                if (command == "room_info") {
                    // rooms_info;room1:uuid;room2:uuid
                    vector<tuple<boost::uuids::uuid, string>> _roomsInfo;
                    _roomsInfo = _RoomManager->getRoomsInfo();
                }
                if (command == "players_info") {
                    vector<tuple<boost::uuids::uuid, string>> _playersInfo;
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid roomUuid = gen(param1);
                    _playersInfo = _RoomManager->getPlayersInfoByRoomId(roomUuid);
                }
                if (command == "player_ready") {
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid playerUuid = gen(param1);
                    _RoomManager->setPlayerReady(playerUuid);
                }
                if (command == "player_not_ready") {
                    boost::uuids::string_generator gen;
                    boost::uuids::uuid playerUuid = gen(param1);
                    _RoomManager->setPlayerNotReady(playerUuid);
                }
            }

            void handleMessage(std::string message)
            {
                std::string result("");
                std::vector<std::string> tokens;
                boost::split(tokens, message, boost::is_any_of(":;"));
                std::string command;
                std::string param1;
                std::string param2;

                if (tokens.size() >= 2) {
                    command = tokens[0];
                    param1 = tokens[1];
                    param2 = "";
                    if (tokens.size() == 3)
                        param2 = tokens[2];
                }
                callFunctions(command, param1, param2);
            }

            boost::asio::io_service &ioService;
        private:
            void startAccept() {
                auto newConnection = std::make_shared<tcp::socket>(ioService);
                m_acceptor.async_accept(*newConnection, [this, newConnection](boost::system::error_code error) {
                    if (!error) {
                        std::cout << "Accepted connection from: " << newConnection->remote_endpoint() << std::endl;
                        m_clients.insert(newConnection->remote_endpoint());
                        startRead(newConnection);
                    }
                    startAccept();
                });
            }

            void startRead(std::shared_ptr<tcp::socket> socket) {
                auto buffer = std::make_shared<boost::asio::streambuf>();
                async_read_until(*socket, *buffer, '|', [this, socket, buffer](boost::system::error_code error, std::size_t bytesRead) {
                    if (!error) {
                        std::istream input(buffer.get());
                        std::string message;
                        std::getline(input, message);
                        std::cout << "Received message: " << message << std::endl;
                        handleMessage(message);
                        startRead(socket);
                    }
                    else {
                        std::cout << "Error while reading: " << error.message() << std::endl;
                    }
                });
            }

            RoomManager *_RoomManager;
            tcp::acceptor m_acceptor;
            std::set<tcp::endpoint> m_clients;
            io_service _ioService;
        };
}
