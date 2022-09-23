#include <iostream>
#include <asio.hpp>

#include "TcpClient.hpp"

void ReadCallback(std::array<char, 1024>& buf) {
    std::cout << "Message from server: " << std::string(buf.data(), buf.size());
}

int main(int argc, char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }
        
        asio::io_context io_context;
        TcpClient client(io_context, argv[1], argv[2], &ReadCallback);
        client.start();
        std::thread t([&io_context]() { io_context.run(); });

        while (true) {
            char* data = new char[1024];
            std::cout << "Message to send: ";
            std::cin.getline(data, 1024);
            data[strlen(data)] = '\n';
            
            client.Write(data);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
