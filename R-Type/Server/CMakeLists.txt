# Server CMake

cmake_minimum_required(VERSION 2.8)

set (PROJECT "rtype_server")

set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

project(${PROJECT} CXX)

if (WIN32)
    set (CMAKE_CXX_FLAGS_DEBUG "-Od -DEBUG -ZI")
    set (CMAKE_CXX_FLAGS_RELEASE "-O2")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W3")
else ()
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-ignored-qualifiers -std=c++11")
endif ()

# SRCDIRS
set (MAINSRCDIR "./Sources")
set (NETSRCDIR "../Network/Sources")
set (GAMEPLAYSRCDIR "../GamePlay/Sources")
set (COMMONSRCDIR "../Common/Source")
set (MARCSRCDIR "../Plugin/PlayerShip")

# INCDIRS
set (MAININCDIR "./Includes")
set (NETINCDIR "../Network/Headers")
set (GAMEPLAYINCDIR "../GamePlay/Headers")
set (COMMONINCDIR "../Common/Header")
set (MARCINCDIR "../Plugin/PlayerShip")

include_directories(${MAININCDIR} ${NETINCDIR} ${GAMEPLAYINCDIR} ${COMMONINCDIR} ${MARCINCDIR})

set (MARCSRCS
  ${MARCSRCDIR}/PlayerShip.cpp)

set (MAINSRCS
    ${MAINSRCDIR}/main.cpp
    ${MAINSRCDIR}/Server.cpp
    ${MAINSRCDIR}/ServerHandlers.cpp
    ${MAINSRCDIR}/Client.cpp
    ${MAINSRCDIR}/Player.cpp
    ${MAINSRCDIR}/GameSession.cpp
    ${MAINSRCDIR}/Interrupt.cpp
    ${MAINSRCDIR}/GamePackage.cpp
    ${MAINSRCDIR}/CommandLine.cpp
    ${MAINSRCDIR}/TCPHandler.cpp
    ${MAINSRCDIR}/Scores.cpp
    ${MAINSRCDIR}/PluginStorage.cpp)

set (NETSRCS
    ${NETSRCDIR}/FileManager.cpp
    ${NETSRCDIR}/UDPSocket.cpp
    ${NETSRCDIR}/TCPSocket.cpp
    ${NETSRCDIR}/NetUser.cpp
    ${NETSRCDIR}/Session.cpp
    ${NETSRCDIR}/SessionManager.cpp
    ${NETSRCDIR}/TCPFrame.cpp
    ${NETSRCDIR}/TCPHeader.cpp
    ${NETSRCDIR}/FileManager.cpp
    ${NETSRCDIR}/UDPFrame.cpp
    ${NETSRCDIR}/UDPHeader.cpp
    ${NETSRCDIR}/UDPManager.cpp
    ${NETSRCDIR}/UDPManagerClient.cpp
    ${NETSRCDIR}/TCPManager.cpp
    ${NETSRCDIR}/TCPManagerServer.cpp
    ${NETSRCDIR}/TCPManagerClient.cpp
    ${NETSRCDIR}/TCPMessageFactoryServer.cpp
    ${NETSRCDIR}/TCPMessageFactory.cpp
    ${NETSRCDIR}/NetClientManager.cpp
    ${NETSRCDIR}/NetClient.cpp
    ${NETSRCDIR}/NetServerManager.cpp
    ${NETSRCDIR}/NetServer.cpp
    ${NETSRCDIR}/GameMessage.cpp
    ${NETSRCDIR}/FileManager.cpp
    ${NETSRCDIR}/PlayerAction.cpp)

set (GAMEPLAYSRCS
    ${GAMEPLAYSRCDIR}/GameObject.cpp
    ${GAMEPLAYSRCDIR}/Monster.cpp
    ${GAMEPLAYSRCDIR}/GameContainer.cpp)

set (COMMONSRCS
    ${COMMONSRCDIR}/XML_Level.cpp
    ${COMMONSRCDIR}/tinyxml2.cpp
    ${COMMONSRCDIR}/Vector2D.cpp
    ${COMMONSRCDIR}/LibManager.cpp
    ${COMMONSRCDIR}/LibLoader.cpp
    ${COMMONSRCDIR}/ResourcesPackageServer.cpp
    ${COMMONSRCDIR}/ResourcesPackage.cpp
    ${COMMONSRCDIR}/Level.cpp)

set (${PROJECT}_SOURCES
    ${MAINSRCS}
    ${NETSRCS}
    ${GAMEPLAYSRCS}
    ${MARCSRCS}
    ${COMMONSRCS})

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

if (NOT WIN32)
    target_link_libraries(${PROJECT} pthread dl)
endif ()
