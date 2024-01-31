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
    Nutckracker/Renderer/Camera.cpp
    Nutckracker/Renderer/GameObject.cpp
    Nutckracker/Renderer/GraphicsContext.cpp
    Nutckracker/Renderer/Model.cpp
    Nutckracker/Renderer/Pipeline.cpp
    Nutckracker/Renderer/RenderBackend.cpp
    Nutckracker/Renderer/Renderer.cpp
    Nutckracker/Renderer/RendererAPI.cpp
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

