#include "nkpch.h"
#include "ImGuiLayer.h"

//#include "imgui.h"
//#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/vec4.hpp"

#include "Nutckracker/Application.hpp"

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
		#if 0
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		#endif
    }

    void ImGuiLayer::End()
    {
		#if 0
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
		#endif
    }

    void ImGuiLayer::OnImGuiRender()
    {
		#if 0
		NK::Application& app = Application::Get();
		NK::Camera* camera = app.GetCamera();

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

		float camera_position[3], camera_rotation[3];
		camera_position[0] = camera->GetCameraPosition().x;
		camera_position[1] = camera->GetCameraPosition().y;
		camera_position[2] = camera->GetCameraPosition().z;

		camera_rotation[0] = camera->GetCameraRotation().x;
		camera_rotation[1] = camera->GetCameraRotation().y;
		camera_rotation[2] = camera->GetCameraRotation().z;

		// Changing view and projection matrices components
		if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f))
        {
            camera->SetPositon(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
        }
        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f))
        {
            camera->SetRotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        }

		float NearPlane = *camera->GetPtrToNearPlane();
		if(ImGui::SliderFloat("Camera Near Plane", &NearPlane, 0.1f, 10.0f))
		{
			camera->SetNearPlane(NearPlane);
		}
		float FarPlane = *camera->GetPtrToFarPlane();
		if(ImGui::SliderFloat("Camera Far Plane", &FarPlane, 1.f, 100.f))
		{
			camera->SetFarPlane(FarPlane);
		}
		float FOV = *camera->GetPtrToFOV();
		if(ImGui::SliderFloat("Camera FOV", &FOV, 1.f, 120.f))
		{
			camera->SetFOV(FOV);
		}

		float lightParams[4];
		lightParams[0] = app.GetAmbient();
		lightParams[1] = app.GetDiffuse();
		lightParams[2] = app.GetSpecular();
		lightParams[3] = app.GetShininess();
		if (ImGui::SliderFloat4("Light Parameters", lightParams, 0, 10.f))
        {
			app.SetAmbient(lightParams[0]);
			app.SetDiffuse(lightParams[1]);
			app.SetSpecular(lightParams[2]);
			app.SetShininess(lightParams[3]);
        }

		ImGui::Checkbox("Perspective camera", app.GetCameraMode());
		#endif
    }
}
