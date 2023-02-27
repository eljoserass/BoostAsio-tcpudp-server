#include "ClientServer.hpp"

using namespace ClientController;

TCPClient::TCPClient(std::string host, int port) : _socket(_io_service)
{
    _socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(host.c_str()), port));
    _isConnected = true;
    commandFromServer = std::make_shared<std::string>(std::string("null"));
}

void listen_from_TCP_server(tcp::socket& socket ,std::shared_ptr<bool>& isTcpRunnig, std::shared_ptr<ClientData>& clientData)
{
    boost::asio::streambuf buff;
    boost::system::error_code error;
    std::string data;
    while (*isTcpRunnig) {
        boost::asio::read_until(socket, buff, "\0", error);
        if (error == boost::asio::error::eof) {
            *isTcpRunnig = false;
            std::cout << "Connection closed by server" << std::endl;
        }
        std::string data = boost::asio::buffer_cast<const char *>(buff.data());
        std::cout << data << std::endl; 
        // aqui parsear respuesta, rellenar clase con datos
        
    }
}

void TCPClient::run(std::shared_ptr<bool>& isTcpRunning, std::shared_ptr<ClientData>& clientData)
{
    isTcpRunning_ = isTcpRunning;
    _send("hola from client");
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

void TCPClient::_send(const std::string &message) {
    boost::system::error_code error;
    boost::asio::write(_socket, boost::asio::buffer(message), error);

    if (!_isConnected) {
        std::cout << "Error: Not connected to server" << std::endl;
    } else if (!error)
        std::cout << "Client sent: " << message << std::endl;
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
    std::cout << data << std::endl;
}