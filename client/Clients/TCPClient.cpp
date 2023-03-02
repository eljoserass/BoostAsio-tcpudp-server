#include "ClientServer.hpp"
#include <boost/bind/bind.hpp>
using namespace ClientController;

TCPClient::TCPClient(std::string host, int port) : _socket(_io_service)
{
    _socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(host.c_str()), port));
    _isConnected = true;
    commandFromServer = std::make_shared<std::string>(std::string("null"));
}

bool stringToBool(std::string target) {
    if (target == "true")
        return true;
    return false;
}

void listen_from_TCP_server(tcp::socket& socket ,std::shared_ptr<bool>& isTcpRunnig, std::shared_ptr<ClientData>& clientData)
{
    boost::asio::streambuf buff;
    boost::system::error_code error;
    std::string data;
    std::string past_data;
    while (*isTcpRunnig) {

        boost::asio::read_until(socket, buff, "\0", error);
        if (error == boost::asio::error::eof) {
            *isTcpRunnig = false;
            std::cout << "Connection closed by server" << std::endl;
        }
        std::string data = boost::asio::buffer_cast<const char *>(buff.data());
        std::vector<std::string> commands_response;
        std::vector<std::string> commands_response_variables;
        if (data != past_data) {
            std::cout << "received from server:" << data << std::endl; 
            past_data = data; // aqui commented
            boost::split(commands_response, data, boost::is_any_of(";"));
            if (commands_response[0] == "new_player") {
                clientData->clientId = commands_response[1];
            }
            if (commands_response[0] == "room_info") {
                clientData->currentAvailableRooms.clear();
                for (int i = 1; i < commands_response.size(); i++)
                    boost::split(commands_response_variables, commands_response[i], boost::is_any_of(":"));
                    clientData->currentAvailableRooms.push_back(std::make_tuple(commands_response_variables[0], commands_response_variables[1]));
            }
            if (commands_response[0] == "players_info") {
                // fix this to be boolean !!!!!!!!!!!!!!!!!!!!
                clientData->currentRoomPlayersName.clear();
                for (int i = 1; i < commands_response.size(); i++)
                    boost::split(commands_response_variables, commands_response[i], boost::is_any_of(":"));
                    clientData->currentRoomPlayersName.push_back(commands_response_variables[0]);
            }
            if (commands_response[0] == "player_ready") {
                clientData->isReady = true;
            }
            if (commands_response[0] == "player_not_ready") {
                clientData->isReady = false;
            }
            if (commands_response[0] == "add_player_room") {
                clientData->currentRoomID =  commands_response[1];
                clientData->isInRoom = true;
            }
            if (commands_response[0] == "remove_player_room") {
                clientData->currentRoomID =  "";
                clientData->isInRoom = false;
            }
            if (commands_response[0] == "gameStarted") {
                clientData->isInGame = true;
                clientData->udpPort = commands_response[0];
            }
        }
    }
}

void TCPClient::run(std::shared_ptr<bool>& isTcpRunning, std::shared_ptr<ClientData>& clientData)
{
    isTcpRunning_ = isTcpRunning;
    listen_thread_ = std::thread(listen_from_TCP_server, std::ref(_socket), std::ref(isTcpRunning), std::ref(clientData));
    // listen_thread_.join();
}

void TCPClient::setIsConnected(bool value)
{
    _isConnected = value;
}

void TCPClient::joinThread()
{
    listen_thread_.join();
}

std::string TCPClient::getCommandFromServer()
{
    return (*commandFromServer);
}

void handler( boost::shared_ptr<std::vector<char> > data )
{
}

void TCPClient::_send(const std::string &message) {
    // boost::system::error_code error;
    // boost::asio::write(_socket, boost::asio::buffer(message), error);
    boost::shared_ptr<std::vector<char> > data(new std::vector<char>);
    std::copy(message.begin(), message.end(), std::back_inserter(*data));
    _socket.async_send(boost::asio::buffer(*data), boost::bind(handler,data));
    // if (!_isConnected) {
    //     std::cout << "Error: Not connected to server" << std::endl;
    // } else if (!error)
    //     std::cout << "Client sent: " << message << std::endl;
}


void TCPClient::_receive() {
    boost::asio::streambuf buff;
    boost::system::error_code error;
    boost::asio::read_until(_socket, buff, "\0", error);
    if (error == boost::asio::error::eof) {
        _isConnected = false;
        std::cout << "Connection closed by server" << std::endl;
    }
    std::string data = boost::asio::buffer_cast<const char *>(buff.data());
    // std::cout << data << std::endl;
}