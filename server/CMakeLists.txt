cmake_minimum_required(VERSION 3.5)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server main.cpp
    server.cpp server.hpp utils.hpp kernel.cpp kernel.hpp client.cpp client.hpp
    connection_delegate.hpp threadsafefile.cpp threadsafefile.hpp)

