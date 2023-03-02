#include "../Include/Servers/TCPServer.hpp"
#include "../Include/Room/RoomManager.hpp"
#include <boost/algorithm/string.hpp>
using namespace Server;

#define PORT 9999

void run_thread(TCPServer &server){
    while (true) {
        server._read();
        server._RoomManager->startGame();
    }
}

TCPServer::TCPServer(boost::asio::io_service &io_service) :
    _io_service(io_service),
    _acceptor(io_service, tcp::endpoint(tcp::v4(), PORT)),
    _clients()
{
    _RoomManager = new RoomManager();
    read_thread = std::thread(run_thread, std::ref(*this));
    start();
}

void TCPServer::start()
{
    while (true) {
        auto client = std::make_shared<tcp::socket>(_io_service);
        _acceptor.accept(*client);
        _clients.push_back(client);

        cout << "New client " << client.get() << " connected" << endl;
       _read();
        
    }
    read_thread.join();
    // auto newConnection = std::make_shared<tcp::socket>(m_acceptor.get_io_service());
    // m_acceptor.async_accept(*newConnection, [this, newConnection](boost::system::error_code error) {
    //     if (!error) {
    //         std::cout << "Accepted connection from: " << newConnection->remote_endpoint() << std::endl;
    //         _clients.push_back(newConnection->remote_endpoint());
    //         startRead(newConnection);
    //     }
    //     startAccept();
    // });
    _RoomManager->_GameManager->joinThreads();
}

void TCPServer::_read()
{
    for (auto &client : _clients) {
        // boost::asio::streambuf buf;
        char data[1024];
        boost::asio::mutable_buffer buffer = boost::asio::buffer(data, 1024);
        boost::asio::streambuf buf;
        boost::asio::read_until (*client, buf, "\0");  // or '\0' if zero terminated
        char* output = (char*)malloc(buf.size());
        memcpy(output, boost::asio::buffer_cast<const void*>(buf.data()), buf.size());  
        handleRead(output, client);
        // client->async_read_some(buffer, [&](boost::system::error_code ec, size_t bytes_transferred) {
        //     if (!ec) {
        //         cout << "Read " << bytes_transferred << " bytes\n";
        //         data[bytes_transferred] = '\0';
        //         handleRead(data, client);
        //     }
        //     else {
        //         cout << "Error: " << ec.message() << "\n";
        //     }
        //     std::cout << "hola" << std::endl;
        // });
        // handleRead(data, client);// try to put it inside
    }
}

void TCPServer::handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    std::cout << "received from client: " << data << std::endl;

    std::string result("");
    std::vector<std::string> first_split;
    std::vector<std::string> second_split;
    std::vector<std::string> third_split;
    char *command;
    char *param;
    char *param2;

    
    boost::split(first_split, std::string(data), boost::is_any_of("|"));
    for (int i = 0; i < first_split.size(); i++) {
        result = "";
        std::cout << "first_split " << first_split[i] <<  " size " << first_split.size() << std::endl;
        
        // second_split.clear();
        // third_split.clear();
        boost::split(second_split, first_split[i], boost::is_any_of(";"));
        boost::split(third_split, second_split[0], boost::is_any_of(":"));
        // char *command = std::strtok(data, ";");
        // char *param = std::strtok(nullptr, ":");
        // char *param2 = std::strtok(nullptr, ":");

        
        command = strdup(second_split[0].c_str());

        if (third_split.size() > 1) {
            param = strdup(third_split[0].c_str());
            param2 = strdup(third_split[1].c_str());
        }

        
        // std::cout << "command: " << command << " param: " << param << " param2 " << param2 << std::endl;

        if (strcmp(command, "new_player") == 0) {
            
            boost::uuids::uuid playerId = _RoomManager->addPlayer(socket, param);
            std::cout << "hola1" << std::endl;
            std::string playerIdStr = boost::uuids::to_string(playerId);
            result = "new_player;" + playerIdStr;
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
            
            const char *roomId = param;
            const char *playerId = param;
            boost::uuids::string_generator gen;
            boost::uuids::uuid roomUuid = gen(roomId);
            boost::uuids::uuid playerUuid = gen(playerId);
            result = "add_player_room;" + boost::lexical_cast<std::string>(roomId);
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
        // if (strcmp(command, "is_room_ready") == 0) {
        //     bool roomIsReady;
        //     const char *roomId = param;
        //     boost::uuids::string_generator gen;
        //     boost::uuids::uuid roomUuid = gen(roomId);
        //     roomIsReady = _RoomManager->isRoomReadyByRoomId(roomUuid);
        // }
        if (strcmp(command, "room_info") == 0) {
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
        std::cout << "send to client" << std::endl;
    }
    data = NULL;
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
    boost::asio::write(*socket, boost::asio::buffer(message), error);
    if (error)
        cerr << "Error sending message to client: " << error.message() << endl;
}
