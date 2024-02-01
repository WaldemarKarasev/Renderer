cmake_minimum_required(VERSION 3.20)

SET (CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "C compiler" FORCE)
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-14" CACHE STRING "C++ compiler" FORCE)
set (CMAKE_CXX_STANDARD 23)
ADD_COMPILE_OPTIONS(-ferror-limit=100)


project(Game_Engine)

include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Engine/Nutckracker)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/imgui)
include_directories(${CMAKE_SOURCE_DIR}/Engine/vendor/glm)




add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/Sandbox)

