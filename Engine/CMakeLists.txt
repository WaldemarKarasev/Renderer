cmake_minimum_required(VERSION 3.20)

project(NK)

add_library(${PROJECT_NAME} SHARED 
    Nutckracker/Application.cpp
    Nutckracker/Log.cpp
    
    Platform/Linux/LinuxWindow.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glfw)

#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/src)
#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/src)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE vendor/glfw
    PRIVATE vendor/spdlog
)

target_precompile_headers(${PROJECT_NAME} PUBLIC nkpch.h)

