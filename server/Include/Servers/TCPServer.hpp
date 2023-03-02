#pragma once
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include "RoomManager.hpp"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;

namespace Server {
    class TCPServer {
        public:
            TCPServer(boost::asio::io_service &io_service);
            void start();

            vector<string> _players;

            void _parse_commands();
            void _read();
            void _sendToClients(const string &message);
            void _sendToClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const string &message);
            void handleRead(char *data, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
            // void start_accept(void) {
            //     tcp_connection::pointer new_connection =
            //     tcp_connection::create(io_context_);

            //     acceptor_.async_accept(new_connection->socket(),
            //         boost::bind(&tcp_server::handle_accept, this, new_connection,
            //         boost::asio::placeholders::error));
            // }

            boost::asio::io_service &_io_service;
            tcp::acceptor _acceptor;
            
            std::vector<std::shared_ptr<tcp::socket>> _clients;
            RoomManager *_RoomManager;
            std::thread read_thread;
    };

    class AsyncTcpServer : public std::enable_shared_from_this<AsyncTcpServer> {
        public:
            AsyncTcpServer(io_service& io_service, int port)
                : ioService(io_service), m_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
            {
                startAccept();
            }
            std::shared_ptr<AsyncTcpServer> getptr() {
                return shared_from_this();
            }
        
            void startAccept() {
                auto newConnection = std::make_shared<tcp::socket>(ioService);
                m_acceptor.async_accept(*newConnection, [this, newConnection](boost::system::error_code error) {
                    if (!error) {
                        std::cout << "Accepted connection from: " << newConnection->remote_endpoint() << std::endl;
                        m_clients.push_back(newConnection);
                        startRead(newConnection);
                    }
                    startAccept();
                });
            }

            void startRead(std::shared_ptr<tcp::socket> socket) {
                auto buffer = std::make_shared<boost::asio::streambuf>();
                async_read_until(*socket, *buffer, '|', [this, socket, buffer](boost::system::error_code error, std::size_t bytesRead) {
                    if (!error) {
                        std::istream input(buffer.get());
                        std::string message;
                        std::getline(input, message);
                        std::cout << "Received message: " << message << " from: "<<  socket->remote_endpoint() << std::endl;
                        sendMessage(socket, "server says wassup");
                        startRead(socket);
                    }
                    else {
                        std::cout << "Error while reading: " << error.message() << std::endl;
                    }
                });
            }

            void handleWrite(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {

            }

            void sendMessage(std::shared_ptr<tcp::socket> socket, std::string message) {
                // boost::asio::async_write(client, boost::asio::buffer(message),
                // boost::bind(&AsyncTcpServer::handleWrite, shared_from_this()));
                boost::asio::async_write(*socket, boost::asio::buffer(message),
                    boost::bind(&AsyncTcpServer::handleWrite, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }

            tcp::acceptor m_acceptor;
            boost::asio::io_service &ioService;
            std::vector<std::shared_ptr<tcp::socket>> m_clients;
            io_service _ioService;
        };

}
