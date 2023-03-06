cmake_minimum_required(VERSION 3.0)

project(client)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

set(SRC
    ClientServer/ClientServer.cpp
    ClientServer/UDPClient.cpp
)

set(HEADERS
    Include/ClientServer.hpp
    Include/UDPClient.hpp
)

add_library(client STATIC ${SRC} ${HEADERS})

target_link_libraries(client Boost::array Boost::asio Boost::uuid)

target_include_directories(client PUBLIC publicInclude)
