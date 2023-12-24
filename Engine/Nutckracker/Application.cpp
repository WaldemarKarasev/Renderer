#include "nkpch.h"
#include "Application.h"

#include <glad/glad.h>
#include <glm/trigonometric.hpp>

#include "Nutckracker/Renderer/Renderer.h"

#include "Input.h"

#include "Nutckracker/Log.h"

namespace NK {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance_ = nullptr;

	Application::Application()
	{	
		NK_CORE_ASSERT(!s_Instance_, "Applicaiton already exists!");
		s_Instance_ = this;
		
		m_Window_ = std::unique_ptr<Window>(Window::Create());
		m_Window_->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer_ = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer_);	

		m_SquareVA_.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
		});
		m_SquareVA_->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA_->SetIndexBuffer(squareIB);

		
		

		std::string blueShaderVertexSrc = R"(
			#version 460
        	layout(location = 0) in vec3 vertex_position;
        	uniform mat4 model_matrix;
			uniform mat4 view_projection_matrix;

        	out vec3 color;
        	void main() {
            	gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
           }
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 460
           	in vec3 color;
           	out vec4 frag_color;

           	void main() {
              	frag_color = vec4(vec4(0.2, 0.3, 0.8, 1.0));
           }
		)";

		m_BlueShader_.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack_.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack_.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));


		for (auto it = m_LayerStack_.end(); it != m_LayerStack_.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
		//NK_TRACE("{0}", e);
	}

	void Application::Run()
	{
		while (m_Running_) 
		{			
			RenderCommand::SetClearColor(m_BackgroundColor_);

			RenderCommand::Clear();

			Renderer::BeginScene(); // Empty implementation

			m_BlueShader_->Bind();

			// model matrix
			glm::mat4 scale_matrix( scale[0], 0,        0,        0,
									0,        scale[1], 0,        0,
									0,        0,        scale[2], 0,
									0,        0,        0,        1);

        	float rotate_in_radians = glm::radians(rotate);
        	glm::mat4 rotate_matrix( cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
                                	-sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
                                	 0,                      0,                      1, 0,
                                	 0,                      0,                      0, 1);

        	glm::mat4 translate_matrix(1,            0,            0,            0,
                                   	   0,            1,            0,            0,
                                   	   0,            0,            1,            0,
                                   	   translate[0], translate[1], translate[2], 1);

        	glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
        	m_BlueShader_->SetMat4("model_matrix", model_matrix);

			// camera matrices. view and projection
			camera.SetPositionRotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
                                     glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
							         camera.SetPtojectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        	m_BlueShader_->SetMat4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

			// drawing m_SquareVA_
			Renderer::Submit(m_SquareVA_);

			Renderer::EndScene();


			for (Layer* layer : m_LayerStack_)
				layer->OnUpdate();

			m_ImGuiLayer_->Begin();
			for (Layer* layer : m_LayerStack_)
				layer->OnImGuiRender();
			m_ImGuiLayer_->End();

			m_Window_->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		//NK_CORE_TRACE("Window closed callback");
		m_Running_ = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		//NK_CORE_TRACE("Window resized callback");
		RenderCommand::SetViewPort(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}