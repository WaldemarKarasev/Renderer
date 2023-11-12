cmake_minimum_required(VERSION 3.20)

SET (CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "C compiler" FORCE)
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-14" CACHE STRING "C++ compiler" FORCE)


project(Game_Engine)

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/Engine/Nutckracker)



add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/Sandbox)

