#include <iostream>
#include <asio.hpp>

#include "TcpServer.hpp"

int main(int, char**) {
    try {
        asio::io_context io_context;
        TcpServer server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
