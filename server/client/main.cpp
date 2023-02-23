#include "ClientServer.hpp"

int main(int argc, char **argv) {
    try {

        ClientServer *clientServer = new ClientServer();
        clientServer->setHost("127.0.0.1");
        clientServer->setTcpPort("9999");

        clientServer->run();
        clientServer->joinThread();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}