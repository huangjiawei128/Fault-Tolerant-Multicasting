cmake_minimum_required(VERSION 3.20)
project(MySimulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(Simulator)

add_executable(Simulator
        Simulator/common.h
        Simulator/common.cpp
        Simulator/CubeNode.h
        Simulator/CubeNode.cpp
        Simulator/Cube.h
        Simulator/Cube.cpp
        Simulator/Routing.h
        Simulator/Routing.cpp
        Simulator/Message.h
        Simulator/Event.h
        Simulator/Event.cpp
        Simulator/main.cpp)

add_executable(test test.cpp)