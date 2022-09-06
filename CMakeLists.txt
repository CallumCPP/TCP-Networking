cmake_minimum_required(VERSION 3.20.0)
project(TCP-Networking VERSION 0.1.0)

include_directories(external/asio/asio/include)

file(GLOB_RECURSE CLIENT_SOURCES "src/Client/*.cpp")
add_executable(TCP-Client ${CLIENT_SOURCES})

file(GLOB_RECURSE SERVER_SOURCES "src/Server/*.cpp")
add_executable(TCP-Server ${SERVER_SOURCES})
