#ifndef SRC_SERVER_TCPCONNECTION_HPP
#define SRC_SERVER_TCPCONNECTION_HPP
#include <asio.hpp>
#include <thread>

std::string MakeDaytimeString() {
	std::time_t now = time(0);
	return std::ctime(&now);
}

class TcpConnection {
public:
	TcpConnection(asio::io_context& io_context) : socket_(io_context) {}

	asio::ip::tcp::socket& socket() {
		return socket_;
	}

	void start() {
		asio::error_code error;

		socket_.async_read_some(asio::buffer(buf),
        	std::bind(&TcpConnection::handle_read, this,
          		std::placeholders::_1));
				
		
	}

private:
	void handle_write(const asio::error_code& error){
		if (!error) {
			socket_.async_read_some(asio::buffer(buf),
				std::bind(&TcpConnection::handle_read, this,
					std::placeholders::_1));
		} else {
			std::cout << error.message() << '\n';
			delete this;
		}
	}

	void handle_read(const asio::error_code& error) {
		if (!error) {
			asio::async_write(socket_,
				asio::buffer(buf),
				std::bind(&TcpConnection::handle_write, this,
				std::placeholders::_1));
		} else {
			std::cout << error.message() << '\n';
			delete this;
		}
	}

	int userID_;
	std::array<char, 1024> buf;
	asio::ip::tcp::socket socket_;
	std::string message_;
};

#endif /* SRC_SERVER_TCPCONNECTION_HPP */
