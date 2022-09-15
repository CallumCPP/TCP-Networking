#ifndef SRC_SERVER_TCPSERVER_HPP
#define SRC_SERVER_TCPSERVER_HPP
#include <asio.hpp>

#include "TcpConnection.hpp"

class TcpServer {
public:
    TcpServer(asio::io_context& io_context)
        : io_context_(io_context), acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 69)) {
        StartAccept();
    }

    void StartAccept() {
        TcpConnection::pointer new_connection =
        TcpConnection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
            std::bind(&TcpServer::HandleAccept, this, new_connection,
            std::placeholders::_1));
    }

    void HandleAccept(TcpConnection::pointer new_connection, const asio::error_code& error) {
        if (!error)
            new_connection->start();

        StartAccept();
    }

    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
};

#endif /* SRC_SERVER_TCPSERVER_HPP */
