cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

if(WIN32)
    set(SRC
    Game/Game.cpp
    Game/GameManagerWin.cpp
    Server/UDPServer.cpp
    )
    set(HEADER
    Include/Game/GameWin.hpp
    Include/Game/GameManagerWin.hpp
    Include/Server/UDPServerWin.hpp
)
else()
    set(SRC
    Game/Game.cpp
    Game/GameManager.cpp
    Server/UDPServer.cpp
    )
    set(HEADER
    Include/Game/Game.hpp
    Include/Game/GameManager.hpp
    Include/Server/UDPServer.hpp
)
endif()

add_library(server STATIC ${SRC} ${HEADER})

target_link_libraries(server PUBLIC Boost::array Boost::asio Boost::uuid)

target_include_directories(server PUBLIC publicInclude)