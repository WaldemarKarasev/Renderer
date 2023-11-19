cmake_minimum_required(VERSION 3.20)

project(glad)

add_library(${PROJECT_NAME} SHARED 
    src/glad.c
)

