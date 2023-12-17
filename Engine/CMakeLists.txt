cmake_minimum_required(VERSION 3.20)

project(NK)

add_library(${PROJECT_NAME}  
    Nutckracker/Application.cpp
    Nutckracker/Layer.cpp
    Nutckracker/LayerStack.cpp
    Nutckracker/Log.cpp

    Nutckracker/ImGui/ImGuiLayer.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp 
    vendor/imgui/backends/imgui_impl_opengl3.cpp

    Nutckracker/Renderer/Buffer.cpp
    Nutckracker/Renderer/RenderCommand.cpp
    Nutckracker/Renderer/RendererAPI.cpp
    Nutckracker/Renderer/Renderer.cpp
    Nutckracker/Renderer/Shader.cpp
    Nutckracker/Renderer/VertexArray.cpp
    
    Platform/CrossPlatform/GLFWInput.cpp
    Platform/CrossPlatform/GLFWWindow.cpp

    
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp



)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glm)


#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/src)
#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/src)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE glfw
    PRIVATE spdlog
    PRIVATE glad
    PRIVATE imgui
    PRIVATE glm
)

target_precompile_headers(${PROJECT_NAME} PUBLIC nkpch.h)

