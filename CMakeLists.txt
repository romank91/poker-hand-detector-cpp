cmake_minimum_required(VERSION 3.16)

project(poker-hand-detector LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# SET(BUILD_MODE Debug)
# SET(CMAKE_CXX_FLAGS "-Wall")
# SET(CMAKE_CXX_FLAGS_DEBUG "-g")
# SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(app)