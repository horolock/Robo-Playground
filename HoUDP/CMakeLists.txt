cmake_minimum_required(VERSION 3.16)
project(houdp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    message(STATUS "Building on macOS")
elseif(UNIX)
    message(STATUS "Building on Linux")
endif()

add_executable(sender src/sender.cpp)
add_executable(receiver src/receiver.cpp)