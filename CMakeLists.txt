cmake_minimum_required(VERSION 3.13)
project(tic_tac_toe)

set(CMAKE_CXX_COMPILER "D:/new_mingw64/mingw64/bin/g++.exe" )
set(CMAKE_C_COMPILER "D:/new_mingw64/mingw64/bin/gcc.exe" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(TicTacToe_Server server.cpp server.h board.cpp board.h helper.h)
target_link_libraries(TicTacToe_Server pthread wsock32)

add_executable(TicTacToe_Client client.cpp client.h board.cpp board.h helper.h)
target_link_libraries(TicTacToe_Client pthread wsock32)