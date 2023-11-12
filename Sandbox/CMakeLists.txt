cmake_minimum_required(VERSION 3.20)
project(sandboxApp)

add_executable(${PROJECT_NAME} src/SandboxApp.cpp)

target_link_libraries(${PROJECT_NAME} NK)



# This if we building from this folder 
#add_executable(${PROJECT_NAME} src/SandboxApp.cpp)
#include_directories(../Engine)
#add_subdirectory(/home/pingvinus/prog/game_eng/Renderer/Engine Engine)
#target_link_libraries(${PROJECT_NAME} NK)