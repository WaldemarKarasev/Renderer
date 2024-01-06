#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Nutckracker/Events/Event.h"

#include "Nutckracker/ImGui/ImGuiLayer.h"
#include "Nutckracker/LayerStack.h"

#include "Nutckracker/Renderer/Camera.h"
#include "Nutckracker/Renderer/Buffer.h"
#include "Nutckracker/Renderer/Shader.h"
#include "Nutckracker/Renderer/Texture.h"
#include "Nutckracker/Renderer/UniformBuffer.h"
#include "Nutckracker/Renderer/VertexArray.h"

#include "glm/vec4.hpp"


#include "Window.h"




namespace NK {

namespace tmp_detail {
	// for blueShader ubo's
	struct BlueCameraData
	{
		glm::mat4 model_matrix;
		glm::mat4 view_projection_matrix;
		int current_frame;
	};

	struct BlueLightData 
	{
		glm::vec3 camera_position;
		float padding0;
		glm::vec3 light_position;
		float padding1;
		glm::vec3 light_color;
		//float c; 
		float ambient_factor = 0.1f;
		float diffuse_factor = 1.0f;
		float specular_factor = 0.5f;
		float shininess = 32.f;
	};

	// for lightShader ubo's
	struct LightCameraData
	{
		glm::mat4 model_matrix;
		glm::mat4 view_projection_matrix;
	};

	struct LightColor
	{
		glm::vec3 light_color;
	};	
	}

	class NK_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get()  { return *s_Instance_; }

		inline Window& GetWindow() { return *m_Window_; }

		// Testing object manipulations at runtime
		//backgound colors
		inline glm::vec4& GetBackgroundColor() { return m_BackgroundColor_; };
		//model matrix
		inline float* GetScale() 		{ return scale; };
		inline float* GetRotation() 	{ return &rotate; };
		inline float* GetTranslation() 	{ return translate; };
		// view and projection matrices
		inline Camera* GetCamera() {return &camera;}
		inline float* GetCameraPosition() { return camera_position; }
		inline float* GetCameraRotation() { return camera_rotation; }
		inline bool* GetCameraMode() 	  { return &perspective_camera; }
	private:
		// Events handlers
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window_;
		ImGuiLayer* m_ImGuiLayer_;
		bool m_Running_ = true;
		LayerStack m_LayerStack_;

		std::shared_ptr<Shader> m_BlueShader_;
		std::shared_ptr<VertexArray> m_SquareVA_;
		tmp_detail::BlueCameraData m_BlueCameraData_{};
		tmp_detail::BlueLightData m_BlueLightData_{};
		std::shared_ptr<UniformBuffer> m_BlueCameraUniformBuffer_;
		std::shared_ptr<UniformBuffer> m_BlueLightUniformBuffer_;
	private:
		// Temporaty members for testing simple transformation of rendered objects and gui manipulations at runtime
	
		// Changing background colors
		glm::vec4 m_BackgroundColor_ = {0.2f, 0.2f, 0.2f, 0.f};

		// Model matrix
		float scale[3] = {1.0f, 1.0f, 1.0f};
		float rotate = 0.0f;
		float translate[3] = {0.0f, 0.0f, 0.0f};

		// View and Projection matrices manipulation
		float camera_position[3] = { 0.f, 0.f, 1.f };
    	float camera_rotation[3] = { 0.f, 0.f, 0.f };
    	bool perspective_camera = true;
    	Camera camera{glm::vec3(-5., 0., 0.)};

		// Textures
		unsigned int m_TextureHandle_;
		const unsigned int m_TextureWidth_ = 1000;
		const unsigned int m_TextureHeight_ = 1000;
		const unsigned int m_TextureChannels = 3;
		int m_Frame_ = 0;
		
		std::shared_ptr<Texture2D> m_SmileTexture_;
		std::shared_ptr<Texture2D> m_QuadsTexture_;

		//----------Light Model-----------//
		float light_source_position[3] = { 0.f, 2.f, 0.f };
        float light_source_color[3] = { 1.f, 1.f, 1.f };
        float ambient_factor = 0.1f;
        float diffuse_factor = 1.0f;
        float specular_factor = 0.5f;
        float shininess = 32.f;
	public:
		float GetAmbient() { return ambient_factor; }
		float GetDiffuse() { return diffuse_factor; }
		float GetSpecular() { return specular_factor; }
		float GetShininess() { return shininess; }
		void SetAmbient(float value) { ambient_factor = value; }
		void SetDiffuse(float value) { diffuse_factor = value; }
		void SetSpecular(float value) { specular_factor = value; }
		void SetShininess(float value) { shininess = value; }
	private:
		std::shared_ptr<Shader> m_LightSourceShader_;
		std::shared_ptr<VertexArray> m_LightSourceIceCube_;
		tmp_detail::LightCameraData m_LightCameraData_{};
		tmp_detail::LightColor m_LightColor_{};
		std::shared_ptr<UniformBuffer> m_LightCameraDataUniformBuffer_;
		std::shared_ptr<UniformBuffer> m_LightColorUniformBuffer_;
		//std::shared_ptr<VertexArray> m_VertexArray_;
		//--------------------------------//

	private:
		static Application* s_Instance_;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}