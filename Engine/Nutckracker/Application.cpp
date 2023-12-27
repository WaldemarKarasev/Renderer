#include "nkpch.h"
#include "Application.h"

#include <glad/glad.h>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Nutckracker/Renderer/Renderer.h"

#include "Input.h"

#include "Nutckracker/Log.h"


// for testing texture generation
#include "ApplicationTextures.h"

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

		float squareVertices[5 * 8] = {
	        // front
	        -1.0f, -1.f, -1.f,   1.f, 0.f,
	        -1.0f,  1.f, -1.f,   0.f, 0.f,
	        -1.0f, -1.f,  1.f,   1.f, 1.f,
	        -1.0f,  1.f,  1.f,   0.f, 1.f,

	        // back
	         1.0f, -1.f, -1.f,   1.f, 0.f,
	         1.0f,  1.f, -1.f,   0.f, 0.f,
	         1.0f, -1.f,  1.f,   1.f, 1.f,
	         1.0f,  1.f,  1.f,   0.f, 1.f
		};	

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "vertex_position"},
			{ShaderDataType::Float2, "texture_coord"}
		});
		m_SquareVA_->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6 * 6] = { 
			0, 1, 2, 3, 2, 1, // front
			4, 5, 6, 7, 6, 5, // back
			0, 4, 6, 0, 2, 6, // right
			1, 5, 3, 3, 7, 5, // left
			3, 7, 2, 7, 6, 2, // top
			1, 5, 0, 5, 0, 4  // bottom	
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA_->SetIndexBuffer(squareIB);

		
		

		std::string blueShaderVertexSrc = R"(
			#version 460
        	layout(location = 0) in vec3 vertex_position;
			//layout(location = 1) in vec3 colors;
			layout(location = 1) in vec2 texture_coord;

        	uniform mat4 model_matrix;
			uniform mat4 view_projection_matrix;
			uniform int current_frame;

			//out vec3 color;
			out vec2 tex_coord_smile;
           	out vec2 tex_coord_quads;
        	void main() {
			  //color = colors;
              tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
           }
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 460
			//in vec3 color;
			in vec2 tex_coord_smile;
    		in vec2 tex_coord_quads;

			layout (binding = 0) uniform sampler2D InTexture_Smile;
        	layout (binding = 1) uniform sampler2D InTexture_Quads;

			out vec4 frag_color;

           	void main() {
              	//frag_color = vec4(color, 1.0);
				frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
           }
		)";

		m_BlueShader_.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

		
		
		unsigned char* data = new unsigned char[m_TextureWidth_ * m_TextureHeight_ * m_TextureChannels];
		
		generate_smile_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_SmileTexture_ = std::make_shared<Texture2D>(data, m_TextureWidth_, m_TextureHeight_);
		m_SmileTexture_->Bind(0);
		
		generate_quads_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_QuadsTexture_ = std::make_shared<Texture2D>(data, m_TextureWidth_, m_TextureHeight_);
		m_QuadsTexture_->Bind(1);

		delete[] data;

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
	}

	void Application::Run()
	{
		std::array<glm::vec3, 5> positions = {
				glm::vec3(-2.f, -2.f, -4.f),
				glm::vec3(-5.f,  0.f,  3.f),
				glm::vec3( 2.f,  1.f, -2.f),
				glm::vec3( 4.f, -3.f,  3.f),
				glm::vec3( 1.f, -7.f,  1.f)
		};

		RenderCommand::EnableDepthTest();	
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
			//m_BlueShader_->SetInt("current_frame", m_Frame_++);

			camera.SetPtojectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        	m_BlueShader_->SetMat4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

			// drawing m_SquareVA_
			Renderer::Submit(m_SquareVA_);

			for (const glm::vec3& current_position : positions)
            {
                glm::mat4 translate_matrix(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    current_position[0], current_position[1], current_position[2], 1);
                
				m_BlueShader_->SetMat4("model_matrix", translate_matrix);
                Renderer::Submit(m_SquareVA_);
            }

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