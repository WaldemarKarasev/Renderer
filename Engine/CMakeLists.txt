cmake_minimum_required(VERSION 3.20)

project(NK)



# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()


add_library(${PROJECT_NAME}  
    Nutckracker/Application.cpp
    Nutckracker/Layer.cpp
    Nutckracker/LayerStack.cpp
    Nutckracker/Log.cpp

    Nutckracker/ImGui/ImGuiLayer.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp 
    vendor/imgui/backends/imgui_impl_opengl3.cpp

    Nutckracker/Renderer/Buffer.cpp
    Nutckracker/Renderer/Camera.cpp
    Nutckracker/Renderer/GameObject.cpp
    Nutckracker/Renderer/GraphicsContext.cpp
    Nutckracker/Renderer/Model.cpp
    Nutckracker/Renderer/Pipeline.cpp
    Nutckracker/Renderer/RenderBackend.cpp
    Nutckracker/Renderer/Renderer.cpp
    Nutckracker/Renderer/RendererAPI.cpp
    Nutckracker/Renderer/RenderSystem.cpp
    Nutckracker/Renderer/Shader.cpp
    Nutckracker/Renderer/Texture.cpp
    Nutckracker/Renderer/UniformBuffer.cpp
    
    Platform/CrossPlatform/GLFWInput.cpp
    Platform/CrossPlatform/GLFWWindow.cpp

    
    #Platform/OpenGL/OpenGLBuffer.cpp
    #Platform/OpenGL/OpenGLContext.cpp
    #Platform/OpenGL/OpenGLRendererAPI.cpp
    #Platform/OpenGL/OpenGLShader.cpp
    #Platform/OpenGL/OpenGLTexture.cpp
    #Platform/OpenGL/OpenGLUniformBuffer.cpp
    #Platform/OpenGL/OpenGLVertexArray.cpp

    Platform/Vulkan/RenderSystems/PointLightSystem.cpp
    Platform/Vulkan/RenderSystems/SimpleRenderSystem.cpp
    Platform/Vulkan/VKContext.cpp
    Platform/Vulkan/VulkanBuffer.cpp
    Platform/Vulkan/VulkanDescriptors.cpp
    Platform/Vulkan/VulkanDevice.cpp
    Platform/Vulkan/VulkanModel.cpp
    Platform/Vulkan/VulkanPipeline.cpp
    Platform/Vulkan/VulkanRenderBackend.cpp
    Platform/Vulkan/VulkanShader.cpp
    Platform/Vulkan/VulkanSwapChain.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/imgui)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/vendor/glm)


#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/src)
#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/src)
# linking vendor libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE glfw
    PRIVATE spdlog
    PRIVATE glad
    PRIVATE imgui
    PRIVATE glm
    PRIVATE vulkan
)

# linking std or sdk libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE stdc++fs
    PRIVATE shaderc_combined
    PRIVATE spirv-cross-core 
    PRIVATE spirv-cross-c
    PRIVATE spirv-cross-cpp
    PRIVATE spirv-cross-glsl 
)

target_precompile_headers(${PROJECT_NAME} PUBLIC nkpch.h)

