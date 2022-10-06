#ifndef SRC_SERVER_TCPCONNECTION_HPP
#define SRC_SERVER_TCPCONNECTION_HPP
#include <asio.hpp>
#include <thread>
#include <sqlite3.h>

class TcpConnection {
public:
	TcpConnection(asio::io_context& io_context, sqlite3*& db) : socket_(io_context), db_(db) {}

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
		} else ErrorHandler(error, true);
	}

	void HandleRead(const asio::error_code& error) {
		if (!error) {
			ParseData();
			buf_.fill(0);
		} else ErrorHandler(error, true);
	}

	void ParseData() {
		int bytes = *(int*)buf_.data();
		std::string message = std::string(buf_.data()+sizeof(int), bytes);
		std::cout << "Message from " << userHash_ << ": " << message << '\n';
		if (buf_.size() > sizeof(int)) {
			if (message.at(0) == '!') {
				if (message.at(1) == 'H') {
					userHash_ = message.substr(3, 64);
					
					std::string result;
					char* zErrMsg = 0;
					std::string sql = "INSERT INTO users VALUES ('" + userHash_ + "', 'balls', 0, 0, 0);";
					sqlite3_exec(db_, sql.c_str(), NULL, NULL, &zErrMsg);
					std::cout << sql << '\n';
					std::cout << zErrMsg << '\n';

					sql = "SELECT * FROM users WHERE hash = '" + userHash_ + "';";
					sqlite3_exec(db_, sql.c_str(), NULL, NULL, NULL);

					
					buf_.fill(0); strcpy(buf_.data(), "Hash Accepted");
					asio::async_write(socket_, asio::buffer(buf_),
						std::bind(&TcpConnection::HandleWrite, this,
							std::placeholders::_1));
					return;
				}
			}
		}

		asio::async_write(socket_, asio::buffer(buf_),
			std::bind(&TcpConnection::HandleWrite, this,
				std::placeholders::_1));
	}

	void ErrorHandler(const asio::error_code& error, bool deleteThis) {
		if (error == asio::error::eof) {
			std::cout << "{SYSTEM} Client disconnected safely!\n";
		} else std::cout << error.message() << '\n';

		if (deleteThis) {
			// upload data to database

			delete this;
		}
	}

	std::string message_, userName_, userHash_;
	int gamesPlayed_, gamesWon_, highScore_;
	asio::ip::tcp::socket socket_;
	std::array<char, 1024> buf_;
	sqlite3* db_;
};

#endif /* SRC_SERVER_TCPCONNECTION_HPP */
