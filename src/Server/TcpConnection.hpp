#ifndef SRC_SERVER_TCPCONNECTION_HPP
#define SRC_SERVER_TCPCONNECTION_HPP
#include <asio.hpp>
#include <thread>

class TcpConnection {
public:
	TcpConnection(asio::io_context& io_context) : socket_(io_context) {}

	asio::ip::tcp::socket& socket() {
		return socket_;
	}

	void start() {
		asio::error_code error;

		socket_.async_read_some(asio::buffer(buf_),
        	std::bind(&TcpConnection::HandleRead, this,
          		std::placeholders::_1));
	}

private:
	void HandleWrite(const asio::error_code& error){
		if (!error) {
			socket_.async_read_some(asio::buffer(buf_),
				std::bind(&TcpConnection::HandleRead, this,
					std::placeholders::_1));
		} else {
			std::cout << error.message() << '\n';
			delete this;
		}
	}

	void HandleRead(const asio::error_code& error) {
		if (!error) {
			std::cout << "Message from client: " << std::string(buf_.data(), buf_.size());
			asio::async_write(socket_,
				asio::buffer(buf_),
				std::bind(&TcpConnection::HandleWrite, this,
				std::placeholders::_1));
			buf_.fill(0);
		} else {
			std::cout << error.message() << '\n';
			delete this;
		}
	}

	std::string userHash_;
	std::array<char, 1024> buf_;
	asio::ip::tcp::socket socket_;
	std::string message_;
};

#endif /* SRC_SERVER_TCPCONNECTION_HPP */
