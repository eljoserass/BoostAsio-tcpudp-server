#pragma once
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "RoomManager.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

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
            
            void sendMessage(std::shared_ptr<tcp::socket> socket, std::string message) {
                // boost::asio::async_write(client, boost::asio::buffer(message),
                // boost::bind(&AsyncTcpServer::handleWrite, shared_from_this()));
                std::cout << "message to send " <<  message << std::endl;
                boost::asio::async_write(*socket, boost::asio::buffer(message),
                    boost::bind(&AsyncTcpServer::handleWrite, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }

            void callFunctions(std::vector<std::string> commands, std::shared_ptr<tcp::socket> socket)
            {
                std::string result("");

                for (int i = 0; i < commands.size(); i++) {
                    std::vector<std::string> tokens;
                    boost::split(tokens, commands[i], boost::is_any_of("|:;"));
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
                    if (commands.size() == 1) {
                        command = commands[0];
                    }
                    if (command == "new_player") {
                        result = "new_player;id";
                        // std::cout << "enter in new player" << std::endl;
                        // _RoomManager->addPlayer(); change to new players
                    }
                    if (command == "create_room") {
                        result = "create_room;ok";
                        // std::cout << "enter in create room" << std::endl;
                        _RoomManager->createRoom(param1);
                        // std::cout << "after room manager" << std::endl;
                        // for (auto& room: _RoomManager->getRoomsInfo()) {
                        //     std::cout << "room name: " << get<0>(room) << " uuid: " << get<1>(room);
                        // }
                    }
                    if (command == "delete_room") {
                        
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid roomUuid = gen(param1);
                        _RoomManager->deleteRoomById(roomUuid);
                    }
                    if (command == "add_player_room") {
                        // e.g add_player_room;roomId:playerId
                        result = "add_player_room;" + boost::lexical_cast<std::string>(param1);
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid roomUuid = gen(param1);
                        boost::uuids::uuid playerUuid = gen(param2);
                        _RoomManager->addPlayerToRoom(roomUuid, playerUuid);
                    }
                    if (command == "remove_player_room") {
                        result = "remove_player_room;ok";
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid playerUuid = gen(param1);
                        _RoomManager->removePlayerFromRoom(playerUuid);
                    }
                    if (command == "room_info") {
                        // rooms_info;room1:uuid;room2:uuid
                        // std::cout << "enter in roomInfo" << std::endl;
                        // vector<tuple<boost::uuids::uuid, string>> _roomsInfo;
                        result = "room_info;";
                        for (int i = 0; i < _RoomManager->getRoomsInfo().size(); i++)
                            result += std::get<1>(_RoomManager->getRoomsInfo()[i]) + ":" + boost::lexical_cast<std::string>(std::get<0>(_RoomManager->getRoomsInfo()[i])) + ";";
                        // result = "room_info;" + result.substr(0, result.size() - 1); // + cantidad de players
                        // _roomsInfo = _RoomManager->getRoomsInfo();
                    }
                    if (command == "players_info") {
                        // vector<tuple<boost::uuids::uuid, string>> _playersInfo;
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid roomUuid = gen(param1);
                        result = "players_info;";
                        for (int i = 0; i < _RoomManager->getPlayersInfoByRoomId(roomUuid).size(); i++)
                            result += std::get<1>(_RoomManager->getPlayersInfoByRoomId(roomUuid)[i]) + ":" + boost::lexical_cast<std::string>(std::get<0>(_RoomManager->getPlayersInfoByRoomId(roomUuid)[i])) + ";";
                        // result = "players_info;" + result.substr(0, result.size() - 1);
                        // _playersInfo = _RoomManager->getPlayersInfoByRoomId(roomUuid);
                    }
                    if (command == "player_ready") {
                        result = "player_ready;" + boost::lexical_cast<std::string>(param1) + ":ok";
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid playerUuid = gen(param1);
                        _RoomManager->setPlayerReady(playerUuid);
                    }
                    if (command == "player_not_ready") {
                        result = "player_not_ready;" + boost::lexical_cast<std::string>(param1) + ":ok";
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid playerUuid = gen(param1);
                        _RoomManager->setPlayerNotReady(playerUuid);
                    }
                    if (command == "is_room_ready") {
                        bool roomIsReady;
                        boost::uuids::string_generator gen;
                        boost::uuids::uuid roomUuid = gen(param1);
                        roomIsReady = _RoomManager->isRoomReadyByRoomId(roomUuid);
                        if (roomIsReady == true) {
                            int port = _RoomManager->_GameManager->startGame("room"); // cambiar esto
                            result = "is_room_ready;" + boost::lexical_cast<std::string>(param1) + ":" + boost::lexical_cast<std::string>(port);
                        } else
                            result = "is_room_ready;" + boost::lexical_cast<std::string>(param1) + ":false";
                    }
                    sendMessage(socket, result);
                }
                
            }

            void handleMessage(std::string message, std::shared_ptr<tcp::socket> socket)
            {
                std::vector<std::string> commands;
                size_t pos = 0;
                // std::cout << "handleMessage\n";
                while (pos < message.size()) {
                    size_t end_pos = message.find_first_of("|", pos);
                    if (end_pos == std::string::npos)
                        end_pos = message.size();
                    std::string cmd = message.substr(pos, end_pos - pos);
                    commands.push_back(cmd);
                    pos = end_pos + 1;
                }
                callFunctions(commands, socket);
            }

            void startAccept() {
                auto newConnection = std::make_shared<tcp::socket>(ioService);
                m_acceptor.async_accept(*newConnection, [this, newConnection](boost::system::error_code error) {
                    if (!error) {
                        std::cout << "Accepted connection from: " << newConnection->remote_endpoint() << std::endl;
                        m_clients.push_back(newConnection);
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
                        std::cout << "Received message: " << message << " from: "<<  socket->remote_endpoint() << std::endl;
                        handleMessage(message, socket);
                        startRead(socket);
                    }
                    else {
                        std::cout << "Error while reading: " << error.message() << std::endl;
                    }
                });
            }
            void handleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {

            }

            RoomManager *_RoomManager;
            tcp::acceptor m_acceptor;
            boost::asio::io_service &ioService;
            std::vector<std::shared_ptr<tcp::socket>> m_clients;
            io_service _ioService;
        };
}
