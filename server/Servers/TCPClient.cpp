#include "TCPClient.hpp"

#define PORT 9999
#define IP "127.0.0.1"

Client::Client(boost::asio::io_service &io_service, tcp::socket &socket) :
    _io_service(io_service),
    _socket(socket)
{
    _socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(IP), PORT));
    _isConnected = true;
    start();
}

void Client::start()
{
    // _send("hola from client");
    _send("create_room;room1");
    _send("create_room;room2");
    sleep(1);
    _send("create_room;room2");
    _send("create_room;room2");
    // _send("add_player_to_room;player1");
    // _send("add_player_to_room;player2");
    // _send("see_rooms");
    // _send("see_players");
    // _send("delete_room");
    // _send("see_rooms");
    while (true) {
        _receive();
    }
}


void Client::_send(const string &message)
{
    boost::system::error_code error;
    boost::asio::write(_socket, boost::asio::buffer(message), error);

    if (!_isConnected) {
        cout << "Error: Not connected to server" << endl;
        exit(0);
    } else if (!error)
        cout << "Client sent: " << message << endl;
    // else
    //     cout << "send failed: " << error.message() << endl;
}

void Client::_receive()
{
    boost::asio::streambuf buff;
    boost::system::error_code error;
    boost::asio::read_until(_socket, buff, "\0", error);
    if (error == boost::asio::error::eof) {
        _isConnected = false;
        cout << "Connection closed by server" << endl;
        exit(0);
    }
    // else if (error) {
    //     cout << "Error: " << error.message() << endl;
    //     exit(0);
    // }
    string data = boost::asio::buffer_cast<const char *>(buff.data());
    cout << data << endl;
}


// int main()
// {
//     boost::asio::io_service io_service;
//     tcp::socket socket(io_service);

//     Client client(io_service, socket);
//     return 0;
// }