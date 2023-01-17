cmake_minimum_required(VERSION 3.5)
project(TcpSocket LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test test.cpp tcpsocket.cpp tcpsocket.h)
