#include "../Include/Servers/TCPServer.hpp"
#include "../Include/Room/RoomManager.hpp"
using namespace Server;

#define PORT 9999

TCPServer::TCPServer(boost::asio::io_service &io_service) :
    _io_service(io_service),
    _acceptor(io_service, tcp::endpoint(tcp::v4(), PORT)),
    _clients()
{
    start();
}

void TCPServer::start()
{
    RoomManager* _RoomManager = new RoomManager();
    while (true) {
        auto client = std::make_shared<tcp::socket>(_io_service);
        _acceptor.accept(*client);
        _clients.push_back(client);

        cout << "New client " << client.get() << " connected" << endl;
        _read(_clients);
    }
    _RoomManager->_GameManager->joinThreads();
}

void TCPServer::_read(vector<std::shared_ptr<tcp::socket>> &clients)
{
    for (auto &client : clients) {
        boost::asio::streambuf buf;
        char data[1024];
        boost::asio::mutable_buffer buffer = boost::asio::buffer(data, 1024);
        client->async_read_some(buffer, [](boost::system::error_code ec, size_t bytes_transferred) {
            if (!ec)
                cout << "Read " << bytes_transferred << " bytes\n";
            else
                cout << "Error: " << ec.message() << "\n";
        });
        handleRead(data, client);
    }
}

void TCPServer::handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    string result;
    char *command = std::strtok(data, ";");
    char *param = std::strtok(nullptr, ":");
    char *param2 = std::strtok(nullptr, ":");

    if (strcmp(command, "new_player") == 0) {
        result = "new_player;ok";
        // e.g new_player;player_name
        _RoomManager->addPlayer(socket, param);
    }
    if (strcmp(command, "create_room") == 0) {
        result = "create_room;ok";
        _RoomManager->createRoom(param);
    }
    if (strcmp(command, "delete_room") == 0) {
        result = "delete_room;ok";
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        _RoomManager->deleteRoomById(roomUuid);
    }
    if (strcmp(command, "add_player_room") == 0) {
        // e.g add_player_room;roomId:playerId
        result = "add_player_room;ok";
        const char *roomId = param;
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        boost::uuids::uuid playerUuid = gen(playerId);
        if (param2 != nullptr)
            _RoomManager->addPlayerToRoom(roomUuid, playerUuid);
    }
    if (strcmp(command, "remove_player_room") == 0) {
        result = "remove_player_room;ok";
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid playerUuid = gen(playerId);
        _RoomManager->removePlayerFromRoom(playerUuid);
    }
    if (strcmp(data, "start_game") == 0) {
        result = "start_game;ok";
        _RoomManager->startGame();
    }
    // if (strcmp(command, "is_room_ready") == 0) {
    //     bool roomIsReady;
    //     const char *roomId = param;
    //     boost::uuids::string_generator gen;
    //     boost::uuids::uuid roomUuid = gen(roomId);
    //     roomIsReady = _RoomManager->isRoomReadyByRoomId(roomUuid);
    // }
    if (strcmp(data, "room_info") == 0) {
        // rooms_info;room1:uuid;room2:uuid
        vector<tuple<boost::uuids::uuid, string>> _roomsInfo;
        for (int i = 0; i < _roomsInfo.size(); i++) {
            result += std::get<1>(_roomsInfo[i]) + ":" + boost::lexical_cast<std::string>(std::get<0>(_roomsInfo[i])) + ";";
        }
        result = "room_info;" + result.substr(0, result.size() - 1);
        _roomsInfo = _RoomManager->getRoomsInfo();
    }
    if (strcmp(command, "players_info") == 0) {
        vector<tuple<boost::uuids::uuid, string>> _playersInfo;
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        for (int i = 0; i < _playersInfo.size(); i++) {
            result += std::get<1>(_playersInfo[i]) + ":" + boost::lexical_cast<std::string>(std::get<0>(_playersInfo[i])) + ";";
        }
        result = "players_info;" + result.substr(0, result.size() - 1);
        _playersInfo = _RoomManager->getPlayersInfoByRoomId(roomUuid);
    }
    if (strcmp(command, "player_ready") == 0) {
        result = "player_ready;" + boost::lexical_cast<std::string>(param) + ":ok";
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid playerUuid = gen(playerId);
        _RoomManager->setPlayerReady(playerUuid);
    }
    if (strcmp(command, "player_not_ready") == 0) {
        result = "player_not_ready;" + boost::lexical_cast<std::string>(param) + ":ok";
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid playerUuid = gen(playerId);
        _RoomManager->setPlayerNotReady(playerUuid);
    }
    _sendToClient(socket, result);
}

void TCPServer::_sendToClients(const string &message)
{
    for (auto &client : _clients) {
        boost::system::error_code error;
        boost::asio::write(*client, boost::asio::buffer(message), error);
        if (error)
            cerr << "Error sending message to client: " << error.message() << endl;
    }
}


void TCPServer::_sendToClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string &message)
{
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(message), error);
    if (error)
        cerr << "Error sending message to client: " << error.message() << endl;
}
