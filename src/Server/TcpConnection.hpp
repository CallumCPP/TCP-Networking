#ifndef SRC_SERVER_TCPCONNECTION_HPP
#define SRC_SERVER_TCPCONNECTION_HPP
#include <asio.hpp>

std::string MakeDaytimeString() {
	std::time_t now = time(0);
	return std::ctime(&now);
}

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	typedef std::shared_ptr<TcpConnection> pointer;

	static pointer create(asio::io_context& io_context) {
		return pointer(new TcpConnection(io_context));
	}

	asio::ip::tcp::socket& socket() {
		return socket_;
	}

	void start() {
		message_ = MakeDaytimeString();

		asio::async_write(
			socket_, asio::buffer(message_),
			std::bind(
				&TcpConnection::handle_write, shared_from_this(),
				std::placeholders::_1,
				std::placeholders::_2
			)
		);
	}

private:
	TcpConnection(asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	void handle_write(const asio::error_code& /*error*/,
		size_t /*bytes_transferred*/)
	{
	}

	asio::ip::tcp::socket socket_;
	std::string message_;
};

#endif /* SRC_SERVER_TCPCONNECTION_HPP */
