# PlayerShip CMake

cmake_minimum_required(VERSION 2.8)

get_filename_component(DIRNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set (PROJECT ${DIRNAME})

set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

project(${PROJECT} CXX)

set (PLUGIN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../=plugin/${DIRNAME}")

if (WIN32)
    set (CMAKE_CXX_FLAGS_DEBUG "-Od -DEBUG -ZI")
    set (CMAKE_CXX_FLAGS_RELEASE "-O2")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W3")
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PLUGIN_OUTPUT_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PLUGIN_OUTPUT_DIR})
else ()
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR})
endif ()

# SRCDIRS
set (MAINSRCDIR ".")
set (GAMEPLAYSRCDIR "../../GamePlay/Sources")
set (COMMONSRCDIR "../../Common/Source")
set (NETWORKSRCDIR "../../Network/Sources")

# INCDIRS
set (MAININCDIR ".")
set (GAMEPLAYINCDIR "../../GamePlay/Headers")
set (COMMININCDIR "../../Common/Header")
set (SERVERINCDIR "../../Server/Includes")
set (NETWORKINCDIR "../../Network/Headers")

include_directories(${MAININCDIR} ${GAMEPLAYINCDIR} ${COMMININCDIR} ${SERVERINCDIR} ${NETWORKINCDIR})

set (MAINSRCS
    ${MAINSRCDIR}/PlayerShip.cpp)

set (NETSRCS
    ${NETWORKSRCDIR}/GameMessage.cpp
    ${NETWORKSRCDIR}/PlayerAction.cpp)

set (GAMEPLAYSRCS
    ${GAMEPLAYSRCDIR}/GameContainer.cpp
    ${GAMEPLAYSRCDIR}/GameObject.cpp
    ${GAMEPLAYSRCDIR}/Monster.cpp)

set (COMMONSRCS
    ${COMMONSRCDIR}/Vector2D.cpp
    ${COMMONSRCDIR}/Level.cpp
    ${COMMONSRCDIR}/XML_Level.cpp
    ${COMMONSRCDIR}/tinyxml2.cpp)

set (${PROJECT}_SOURCES
    ${MAINSRCS}
    ${NETSRCS}
    ${COMMONSRCS}
    ${GAMEPLAYSRCS})

add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})

set_target_properties(${PROJECT} PROPERTIES PREFIX "")
