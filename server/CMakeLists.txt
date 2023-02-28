cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

set(SRC
    Game/Game.cpp
    Game/GameManager.cpp
    Player/Player.cpp
    Room/Room.cpp
    Room/RoomManager.cpp
    Servers/TCPServer.cpp
    Servers/UDPServer.cpp
)

set(HEADER
    Include/Game/Game.hpp
    Include/Game/GameManager.hpp
    Include/Player/Player.hpp
    Include/Room/Room.hpp
    Include/Room/RoomManager.hpp
    Include/Servers/TCPServer.hpp
    Include/Servers/UDPServer.hpp
)

add_library(server STATIC ${SRC} ${HEADER})

target_link_libraries(server PUBLIC Boost::array Boost::asio Boost::uuid)

target_include_directories(server PUBLIC publicInclude)