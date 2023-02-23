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
        boost::asio::mutable_buffers_1 buffer = boost::asio::buffer(buf.prepare(100));
        client->async_read_some(buffer, [](boost::system::error_code ec, size_t bytes_transferred) {
            if (!ec)
                cout << "Read " << bytes_transferred << " bytes\n";
            else
                cout << "Error: " << ec.message() << "\n";
        });
        const char *data = boost::asio::buffer_cast<const char *>(buf.data());
        // hacer un split de data para separar por ;
        if (strcmp(data, "newClient;player1") == 0) {
            boost::uuids::uuid playerId;
            playerId = _RoomManager->addPlayer(client, /* variable de comando*/ "player1");
            // myID;playerId 
            // _send(playerId);
        }
        if (strcmp(data, "create_room;room1") == 0)
            _RoomManager->createRoom("room1");
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
