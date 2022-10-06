#ifndef SRC_SERVER_TCPSERVER_HPP
#define SRC_SERVER_TCPSERVER_HPP
#include <asio.hpp>

#include "TcpConnection.hpp"

class TcpServer {
public:
    TcpServer(asio::io_context& io_context) : io_context_(io_context), acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4347)) {
        if (sqlite3_open("database.db", &db_)) std::cout << "Error opening database: " << sqlite3_errmsg(db_) << '\n';
        if (sqlite3_exec(db_, "CREATE TABLE IF NOT EXISTS users (hash TEXT PRIMARY KEY UNIQUE, userName TEXT, highScore INTEGER, gamesPlayed INTEGER, gamesWon INTEGER);", NULL, NULL, NULL)) std::cout << "Error creating table: " << sqlite3_errmsg(db_) << '\n';
        sqlite3_exec(db_, "INSERT INTO users VALUES ('test', 'balls', 0, 0, 0);", NULL, NULL, NULL);
        StartAccept();
    }

    ~TcpServer() {
        sqlite3_close(db_);
    }

private:
    void StartAccept() {
        TcpConnection* new_connection = new TcpConnection(io_context_, db_);

        acceptor_.async_accept(new_connection->socket(),
            std::bind(&TcpServer::HandleAccept, this, new_connection,
            std::placeholders::_1));
    }

    void HandleAccept(TcpConnection* new_connection, const asio::error_code& error) {
        if (!error)
            new_connection->start();

        StartAccept();
    }

    void close() {
        acceptor_.close();
        delete this;
    }

    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    sqlite3* db_;
};

#endif /* SRC_SERVER_TCPSERVER_HPP */
