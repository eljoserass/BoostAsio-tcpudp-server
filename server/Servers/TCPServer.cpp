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
        // hacer un split de data para separar por ;
        if (strcmp(data, "newClient;player1") == 0) {
            boost::uuids::uuid playerId;
            playerId = _RoomManager->addPlayer(client, /* variable de comando*/ "player1");
            // myID;playerId 
            // _send(playerId);
        }
        if (strcmp(data, "create_room;room1") == 0)
            _RoomManager->createRoom("room1");
        if (strcmp(data, "roomsName") == 0) {
            std::string roomsName("");
            for (int i = 0; i < _RoomManager->_roomNames.size(); i++) {
                roomsName += _RoomManager->_roomNames[i];
                roomsName += ";";
            }
            _send(roomsName);
        }
        if (strcmp(data, "roomsId") == 0) {
            std::string roomsId("");
            for (int i = 0; i < _RoomManager->_roomIds.size(); i++) {
                roomsId += _RoomManager->_roomIds[i];
                roomsId += ";";
            }
            _send(roomsId);
        }
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
