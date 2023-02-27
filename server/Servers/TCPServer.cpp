#include "../Include/Servers/TCPServer.hpp"
#include "../Include/Room/RoomManager.hpp"

#define PORT 9999

Server::Server(boost::asio::io_service &io_service) :
    _io_service(io_service),
    _acceptor(io_service, tcp::endpoint(tcp::v4(), PORT)),
    _clients()
{
    start();
}

void Server::start()
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

void Server::_read(vector<std::shared_ptr<tcp::socket>> &clients)
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

void Server::handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    char *command = std::strtok(data, ";");
    char *param = std::strtok(nullptr, ":");
    char *param2 = std::strtok(nullptr, ":");

    if (strcmp(command, "new_player") == 0)
        // e.g new_player;player_name
        _RoomManager->addPlayer(socket, param);
    if (strcmp(command, "create_room") == 0)
        _RoomManager->createRoom(param);
    if (strcmp(command, "delete_room") == 0) {
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        _RoomManager->deleteRoomById(roomUuid);
    }
    if (strcmp(command, "add_player_room") == 0) {
        // e.g add_player_room;roomId:playerId
        const char *roomId = param;
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        boost::uuids::uuid playerUuid = gen(playerId);
        if (param2 != nullptr)
            _RoomManager->addPlayerToRoom(roomUuid, playerUuid);
    }
    if (strcmp(command, "remove_player_room") == 0) {
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid playerUuid = gen(playerId);
        _RoomManager->removePlayerFromRoom(playerUuid);
    }
    if (strcmp(data, "start_game") == 0)
        _RoomManager->startGame();
    if (strcmp(command, "is_room_ready") == 0) {
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        roomIsReady = _RoomManager->isRoomReadyByRoomId(roomUuid);
    }
    if (strcmp(data, "room_info") == 0)
        _roomInfo = _RoomManager->getRoomInfo();
    if (strcmp(command, "players_in_room") == 0) {
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        _players = _RoomManager->getPlayersByRoomId(roomUuid);
    }
    if (strcmp(command, "players_info") == 0) {
        const char *roomId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid roomUuid = gen(roomId);
        _playerInfo = _RoomManager->getPlayersInfoByRoomId(roomUuid);
    }
    if (strcmp(command, "player_ready") == 0) {
        const char *playerId = param;
        boost::uuids::string_generator gen;
        boost::uuids::uuid playerUuid = gen(playerId);
        _RoomManager->getPlayerReady(playerUuid);
    }
}

void Server::_send(const string &message)
{
    for (auto &client : _clients) {
        boost::system::error_code error;
        boost::asio::write(*client, boost::asio::buffer(message), error);
        if (error)
            cerr << "Error sending message to client: " << error.message() << endl;
    }
}
