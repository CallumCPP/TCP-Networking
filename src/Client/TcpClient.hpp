#ifndef SRC_CLIENT_TCPCLIENT_HPP
#define SRC_CLIENT_TCPCLIENT_HPP
#include <asio.hpp>
#include <thread>
#include <iostream>

class TcpClient {
public:
    TcpClient(asio::io_context& io_context, char* host, char* port, void  (*readCallback)(std::array<char, 1024>& buf)) : socket_(io_context), readCallback_(readCallback) {
        asio::error_code error;
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
        asio::connect(socket_, endpoints, error);
        if (error) {
            std::cout << error.message() << '\n';
        }
    }

    void start() {
        asio::async_read(socket_, asio::buffer(buf_),
            std::bind(&TcpClient::HandleRead, this,
                std::placeholders::_1));
    }

    void Write(std::string message) {
        asio::error_code error;
        
        asio::write(socket_, asio::buffer(message), error);
        if (error) std::cout << error.message() << '\n';
    }

    void Write(std::array<char, 1024>& buf) {
        asio::error_code error;
        
        asio::write(socket_, asio::buffer(buf), error);
        if (error) std::cout << error.message() << '\n';
    }
    
private:
    void HandleRead(const asio::error_code& error) {
        if (!error) {
            readCallback_(buf_);
            asio::async_read(socket_,
                asio::buffer(buf_),
                std::bind(&TcpClient::HandleRead, this,
                std::placeholders::_1));
            buf_.fill(0);
        } else {
            std::cout << error.message() << '\n';
            delete this;
        }
    }

    void  (*readCallback_)(std::array<char, 1024>& buf);
    std::array<char, 1024> buf_;
    asio::ip::tcp::socket socket_;
};

#endif /* SRC_CLIENT_TCPCLIENT_HPP */
