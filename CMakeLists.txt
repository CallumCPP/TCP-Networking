cmake_minimum_required(VERSION 3.0.0)
project(TCP-Networking VERSION 0.1.0)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})