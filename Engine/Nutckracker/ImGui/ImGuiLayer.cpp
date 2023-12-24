#include "nkpch.h"
#include "ImGuiLayer.h"

//#include "imgui.h"
//#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/vec4.hpp"

#include "Nutckracker/Application.h"

// Temporary
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace NK {
    ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") 
    {
    
    }

    ImGuiLayer::~ImGuiLayer()
    {
		if (ImGui::GetCurrentContext())
		{
			ImGui::DestroyContext();
		}
    }
    
    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

    }
    
    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin() 
    {
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();

        Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
    }

    void ImGuiLayer::OnImGuiRender()
    {
		NK::Application& app = Application::Get();

        static bool show = true;
		ImGui::ShowDemoWindow(&show);
		

		glm::vec4& color = Application::Get().GetBackgroundColor(); 
		float color_vec[4] = {color.r, color.g, color.b, color.a}; 
		
		ImGui::ColorEdit4("Background Color", color_vec);

		color.r = color_vec[0];
		color.g = color_vec[1];
		color.b = color_vec[2];
		color.a = color_vec[3];

		// Changing model matrix components
		ImGui::SliderFloat3("Scale", app.GetScale(), 0.f, 2.f);
		ImGui::SliderFloat("Rotate", app.GetRotation(), 0.0f, 360.0f);
		ImGui::SliderFloat3("Translate", app.GetTranslation(), -1.0f, 1.0f);

		// Changing view and projection matrices components
		ImGui::SliderFloat3("camera position", app.GetCameraPosition(), -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", app.GetCameraRotation(), 0, 360.f);
        ImGui::Checkbox("Perspective camera", app.GetCameraMode());
		
    }
    
    
    
        
}
