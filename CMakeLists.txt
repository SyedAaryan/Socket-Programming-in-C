cmake_minimum_required(VERSION 3.29)
project(SocketProgramming C)

set(CMAKE_C_STANDARD 11)

add_executable(SocketProgramming device1.c
        device2.c)
