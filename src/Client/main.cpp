#include <iostream>
#include <asio.hpp>

int main(int argc, char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);

        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        while (true) {
            std::array<char, 1024> buf;
            asio::error_code error;

            char* data = new char[1024];

            std::cout << "Message to send: ";
            std::cin.getline(data, 1024);
            data[strlen(data)] = '\n';
            socket.write_some(asio::buffer(data, 1024), error);
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof) {
                std::cout << "{SYSTEM} Connection  closed cleanly by peer!\n";
                break;
            } else if (error) {
                std::cout << "{SYSTEM} Unknown error occurred\n";
                throw asio::system_error(error);
            }

            std::string message("Message from server: "+ std::string(buf.data(), len));
            std::cout << message;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
