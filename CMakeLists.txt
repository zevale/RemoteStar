cmake_minimum_required(VERSION 3.9)
project(RemoteStar)

set(CMAKE_CXX_STANDARD 11)

add_executable(RemoteStar main.cpp SSH.cpp star_client.cpp StarHost.cpp StarHost.h)