#include <iostream>
#include <asio.hpp>

#include "TcpClient.hpp"
#include "SHA256.hpp"

void ReadCallback(std::array<char, 1024>& buf) {
    std::cout << "Message from server: " << std::string(buf.data(), buf.size()) << '\n';
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

        std::cout << "Enter password: ";
        std::string password; std::cin >> password;
        std::string hashedPassword = sha256(password);
        std::cout << "Hashed password: " << hashedPassword.size() << '\n';
        std::string request("!H " + hashedPassword);
        std::array<char, 1024> buf;
        int tmp = request.size();
        std::memcpy(buf.data(), &tmp, sizeof(int));
        std::memcpy(buf.data()+sizeof(int), request.c_str(), request.size());
        client.Write(buf);
        password.clear();

        while (true) {
            std::array<char, 1024> buf;
            std::cout << "Message to send: ";
            std::cin.getline(buf.data(), 1024);
            
            client.Write(buf.data());
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
