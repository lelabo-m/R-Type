# Plugin CMake

cmake_minimum_required(VERSION 2.8)

if (NOT CMAKE_BUILD_TYPE)
    unset(CMAKE_BUILD_TYPE CACHE)
endif ()

add_subdirectory("./Moskito")
add_subdirectory("./PlayerShip")
add_subdirectory("./FireBall")