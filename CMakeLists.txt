cmake_minimum_required(VERSION 3.20.0)
project(TCP-Networking VERSION 0.1.0)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

include_directories(external/asio/asio/include external/sqlite3)
add_library(sqlite3 STATIC external/sqlite3/sqlite3.c external/sqlite3/shell.c)

add_definitions(-DASIO_STANDALONE)

file(GLOB_RECURSE CLIENT_SOURCES "src/Client/*.cpp")
add_executable(TCP-Client ${CLIENT_SOURCES})

file(GLOB_RECURSE SERVER_SOURCES "src/Server/*.cpp")
add_executable(TCP-Server ${SERVER_SOURCES})
target_link_libraries(TCP-Server sqlite3)
