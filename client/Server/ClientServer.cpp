#include "ClientServer.hpp"

ClientServer::ClientServer()
{
    isUDPRunning_ = std::make_shared<bool>(bool(false));
    isTCPRunning_ = std::make_shared<bool>(bool(true));
    clientData = std::make_shared<ClientData>(ClientData());
}

void ClientServer::startUdp(std::string port)
{
    udpClient_ = new UDPClient(host_,port.c_str(), std::ref(gameState));
    udpClient_->run(std::ref(gameState), isUDPRunning_);
}

void ClientServer::startTcp()
{
    std::cout << "startcp" <<std::endl;
    tcpClient_ = new TCPClient(host_, stoi(tcpPort_));
    std::cout << "tcp init" << std::endl;
    tcpClient_->run(isTCPRunning_, clientData);
}

void ClientServer::joinThread(void)
{
    std::cout << "joinThread" << std::endl;
    // puede que pete aqui
    tcpClient_->joinThread();
    // udpClient_->joinThread();
}

void ClientServer::run()
{
    startTcp();
    // parseTCPResponse();
}

void ClientServer::setUdpPort(const char *udpPort)
{
    udpPort_ = udpPort;
}

void ClientServer::setTcpPort(const char *tcpPort)
{
    tcpPort_ = tcpPort;
}

void ClientServer::setHost(const char *host)
{
    host_ = host;
}

void ClientServer::speak_to_UDPserver(udp::socket& socket, udp::endpoint& receiver_endpoint, std::string& message)
{
    socket.send_to(boost::asio::buffer(" message from client outside loop"), receiver_endpoint);
    while (true) {
        sleep(1);
        socket.send_to(boost::asio::buffer(" message from client in loop"), receiver_endpoint);
    }
}

