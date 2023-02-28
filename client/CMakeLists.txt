cmake_minimum_required(VERSION 3.0)

project(client)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include ${CMAKE_BINARY_DIR}/_deps/boost-src/libs/array/include)

set(SRC
    Clients/TCPClient.cpp
    Clients/UDPClient.cpp
    Data/ClientData.cpp
    Server/ClientServer.cpp
)

set(HEADERS
    Include/ClientServer.hpp
    Include/ClientData.hpp
    Include/UDPClient.hpp
    Include/TCPClient.hpp
)

add_library(client STATIC ${SRC} ${HEADERS})

target_link_libraries(client PRIVATE Boost::array Boost::asio Boost::uuid)

target_include_directories(client PUBLIC publicInclude)
