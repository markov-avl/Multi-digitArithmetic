cmake_minimum_required(VERSION 3.17)
project(Multi-digitArithmetic)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp src/longnum.cpp src/messages.cpp src/constants.cpp)