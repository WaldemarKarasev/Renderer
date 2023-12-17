#include "nkpch.h"
#include "Application.h"

//#include <glad/glad.h>
#include "Nutckracker/Renderer/Renderer.h"

#include "Input.h"

//#include "Log.h"

namespace NK {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
				case NK::ShaderDataType::Float:    return GL_FLOAT;
				case NK::ShaderDataType::Float2:   return GL_FLOAT;
				case NK::ShaderDataType::Float3:   return GL_FLOAT;
				case NK::ShaderDataType::Float4:   return GL_FLOAT;
				case NK::ShaderDataType::Mat3:     return GL_FLOAT;
				case NK::ShaderDataType::Mat4:     return GL_FLOAT;
				case NK::ShaderDataType::Int:      return GL_INT;
				case NK::ShaderDataType::Int2:     return GL_INT;
				case NK::ShaderDataType::Int3:     return GL_INT;
				case NK::ShaderDataType::Int4:     return GL_INT;
				case NK::ShaderDataType::Bool:     return GL_BOOL;
			}

			NK_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	Application::Application()
	{	
		NK_CORE_ASSERT(!s_Instance, "Applicaiton already exists!");
		s_Instance = this;
		
		m_Window_ = std::unique_ptr<Window>(Window::Create());
		m_Window_->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer_ = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer_);	

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};


		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		uint32_t indices[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader_.reset(new Shader(vertexSrc, fragmentSrc));

	}


	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDisp	atcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		for (auto it = m_LayerStack_.end(); it != m_LayerStack_.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
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

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running_ = false;
		return true;
	}

	void Application::Run()
	{
		while (m_Running_) 
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1f});
			RenderCommand::Clear();

			Renderer::BeginScene();

			Renderer::Submit(m_SquareVA);
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			m_Shader_->Bind();
			for (Layer* layer : m_LayerStack_)
				layer->OnUpdate();

			m_ImGuiLayer_->Begin();
			for (Layer* layer : m_LayerStack_)
				layer->OnImGuiRender();
			m_ImGuiLayer_->End();



			m_Window_->OnUpdate();
		}
	}

}