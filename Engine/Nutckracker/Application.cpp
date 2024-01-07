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

		float squareVertices[8 * 24] = {
			//    position             normal            UV                  index

			// FRONT
			-1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
			-1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
			-1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
			-1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

			// BACK                                  
			1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
			1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
			1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
			1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

			// RIGHT
			-1.0f,  1.f, -1.f,    0.f,  1.f,  0.f,     0.f, 0.f,              // 8
			1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
			1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
			-1.0f,  1.f,  1.f,    0.f,  1.f,  0.f,     0.f, 1.f,              // 11

			// LEFT
			-1.0f, -1.f, -1.f,    0.f, -1.f,  0.f,     1.f, 0.f,              // 12
			1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
			1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
			-1.0f, -1.f,  1.f,    0.f, -1.f,  0.f,     1.f, 1.f,              // 15

			// TOP
			-1.0f, -1.f,  1.f,    0.f,  0.f,  1.f,     0.f, 0.f,              // 16
			-1.0f,  1.f,  1.f,    0.f,  0.f,  1.f,     1.f, 0.f,              // 17
			1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
			1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

			// BOTTOM
			-1.0f, -1.f, -1.f,   0.f,  0.f, -1.f,     0.f, 1.f,              // 20
			-1.0f,  1.f, -1.f,   0.f,  0.f, -1.f,     1.f, 1.f,              // 21
			1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
			1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
		};	

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "vertex_position"},
			{ShaderDataType::Float3, "normal"},
			{ShaderDataType::Float2, "texture_coord"}
		});
		m_SquareVA_->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6 * 6] = { 
			//0, 1, 2, 3, 2, 1, // front
			//4, 5, 6, 7, 6, 5, // back
			//0, 4, 6, 0, 2, 6, // right
			//1, 5, 3, 3, 7, 5, // left
			//3, 7, 2, 7, 6, 2, // top
			//1, 5, 0, 5, 0, 4  // bottom	
			0,   1,  2,  2,  3,  0, // front
        	4,   5,  6,  6,  7,  4, // back
        	8,   9, 10, 10, 11,  8, // right
        	12, 13, 14, 14, 15, 12, // left
        	16, 17, 18, 18, 19, 16, // top
        	20, 21, 22, 22, 23, 20  // bottom
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA_->SetIndexBuffer(squareIB);

	
		// light source cube init
		m_LightSourceIceCube_.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> lightVB;
		lightVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		lightVB->SetLayout({
			{ShaderDataType::Float3, "vertex_position"},
			{ShaderDataType::Float3, "normal"},
			{ShaderDataType::Float2, "texture_coord"}
		});
		
		m_LightSourceIceCube_->AddVertexBuffer(lightVB);
		std::shared_ptr<IndexBuffer> lightIB;
		lightIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_LightSourceIceCube_->SetIndexBuffer(lightIB);
		//
	
		

		std::string blueShaderVertexSrc = R"(
			#version 450
        	layout(location = 0) in vec3 vertex_position;
			layout(location = 1) in vec3 vertex_normal;
			layout(location = 2) in vec2 texture_coord;

			layout(location = 0) out vec2 tex_coord_smile;
           	layout(location = 1) out vec2 tex_coord_quads;
			layout(location = 2) out vec3 frag_position;
			layout(location = 3) out vec3 frag_normal;

			layout(std140, binding = 0) uniform CameraData
			{
				mat4 model_matrix;
				mat4 view_projection_matrix;
				int current_frame;
			} u_data;

			//mat4 model_matrix;
			//uniform mat4 view_projection_matrix;
			//uniform int current_frame;

        	void main() {
				tex_coord_smile = texture_coord;
				tex_coord_quads = texture_coord + vec2(u_data.current_frame / 1000.f, u_data.current_frame / 1000.f);
				frag_normal = mat3(transpose(inverse(u_data.model_matrix))) * vertex_normal;
				vec4 vertex_position_world = u_data.model_matrix * vec4(vertex_position, 1.0);
				frag_position = vertex_position_world.xyz;
				gl_Position = u_data.view_projection_matrix * vertex_position_world;
           }
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 450
			//in vec3 color;
			layout(location = 0) in vec2 tex_coord_smile;
    		layout(location = 1) in vec2 tex_coord_quads;
			layout(location = 2) in vec3 frag_position;
			layout(location = 3) in vec3 frag_normal;

			layout(binding = 0) uniform sampler2D InTexture_Smile;
        	layout(binding = 1) uniform sampler2D InTexture_Quads;

			layout(std140, binding = 1) uniform LightData
			{
				vec3 camera_position;
				vec3 light_position;
				vec3 light_color;
				float ambient_factor;
				float diffuse_factor;
				float specular_factor;
				float shininess;
			}u_data;

			//uniform vec3 camera_position;
            //uniform vec3 light_position;
            //uniform vec3 light_color;
            //uniform float ambient_factor;
            //uniform float diffuse_factor;
            //uniform float specular_factor;
            //uniform float shininess;

			layout(location = 0) out vec4 frag_color;

           	void main() {
              				
				// ambient
              	vec3 ambient = u_data.ambient_factor * u_data.light_color;
				// diffuse
              	vec3 normal = normalize(frag_normal);
              	vec3 light_dir = normalize(u_data.light_position - frag_position);
              	vec3 diffuse = u_data.diffuse_factor * u_data.light_color * max(dot(normal, light_dir), 0.0);

				// specular
              	vec3 view_dir = normalize(u_data.camera_position - frag_position);
              	vec3 reflect_dir = reflect(-light_dir, normal);
              	float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), u_data.shininess);
              	vec3 specular = u_data.specular_factor * specular_value * u_data.light_color;

				//frag_color = vec4(color, 1.0);
				//frag_color = texture(InTexture_Smile, tex_coord_smile);// * texture(InTexture_Quads, tex_coord_quads);
				frag_color = texture(InTexture_Smile, tex_coord_smile) * vec4(ambient + diffuse + specular, 1.f);

           }
		)";

		m_BlueShader_.reset(Shader::Create("BlueShader", blueShaderVertexSrc, blueShaderFragmentSrc));


    	std::string light_source_vertex_shader =
        	R"(#version 450
        	   layout(location = 0) in vec3 vertex_position;
        	   layout(location = 1) in vec3 vertex_normal;
        	   layout(location = 2) in vec2 texture_coord;
        	   
			   layout(std140, binding = 2) uniform CamData
			   {
				  mat4 model_matrix;
				  mat4 view_projection_matrix;
			   }u_data;

			   //uniform mat4 model_matrix;
        	   //uniform mat4 view_projection_matrix;
        	   void main() {
        	      gl_Position = u_data.view_projection_matrix * u_data.model_matrix * vec4(vertex_position * 0.1f, 1.0);
        	   }
        	)";

    	std::string light_source_fragment_shader =
    	    R"(#version 450
    	       layout(location = 0) out vec4 frag_color;
			   layout(std140, binding = 3) uniform LightCol
			   {
				  vec3 light_color;
			   } u_data;
    	       //uniform vec3 light_color;
    	       void main() {
    	          frag_color = vec4(u_data.light_color, 1.f);
    	       }
    	    )";

		m_LightSourceShader_.reset(Shader::Create("LightSourceShader", light_source_vertex_shader, light_source_fragment_shader));
		
		
		//Texture initialization
		unsigned char* data = new unsigned char[m_TextureWidth_ * m_TextureHeight_ * m_TextureChannels];
		
		TextureSpecification TexSpec;
		TexSpec.Width = m_TextureWidth_;
		TexSpec.Height = m_TextureHeight_;
		TexSpec.GenerateMips = true;
		TexSpec.data = data;
		
		generate_smile_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_SmileTexture_.reset(Texture2D::Create(TexSpec));
		m_SmileTexture_->SetData(data, m_TextureWidth_ * m_TextureHeight_ * m_TextureChannels);
		m_SmileTexture_->Bind(0);
		
		generate_quads_texture(data, m_TextureWidth_, m_TextureHeight_);
		m_QuadsTexture_.reset(Texture2D::Create(TexSpec));
		m_QuadsTexture_->Bind(1);
		
		delete[] data;


		// UniformBuffers initialization
		m_BlueCameraUniformBuffer_.reset(UniformBuffer::Create(sizeof(tmp_detail::BlueCameraData), 0));
		m_BlueLightUniformBuffer_.reset(UniformBuffer::Create(sizeof(tmp_detail::BlueLightData), 1));



		m_LightCameraDataUniformBuffer_.reset(UniformBuffer::Create(sizeof(tmp_detail::LightCameraData), 2));
		m_LightColorUniformBuffer_.reset(UniformBuffer::Create(sizeof(tmp_detail::LightColor), 3));


		m_BlueCameraUniformBuffer_->SetData(&m_BlueCameraData_, sizeof(tmp_detail::BlueCameraData));
		m_BlueLightUniformBuffer_->SetData(&m_BlueLightData_, sizeof(tmp_detail::BlueLightData));
		m_LightCameraDataUniformBuffer_->SetData(&m_LightCameraData_, sizeof(tmp_detail::LightCameraData));
		m_LightColorUniformBuffer_->SetData(&m_LightColor_, sizeof(tmp_detail::LightColor));
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
		RenderCommand::Init();
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
        	
			camera.SetPtojectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			
			m_BlueCameraData_.current_frame++;
			m_BlueCameraData_.model_matrix = model_matrix;
			m_BlueCameraData_.view_projection_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();

			m_BlueLightData_.camera_position = camera.GetCameraPosition();
			m_BlueLightData_.light_position = glm::vec3(light_source_position[0], light_source_position[1], light_source_position[2]);
			m_BlueLightData_.light_color = glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]);
			m_BlueLightData_.ambient_factor = ambient_factor;
			m_BlueLightData_.diffuse_factor = diffuse_factor;
			m_BlueLightData_.specular_factor = specular_factor;
			m_BlueLightData_.shininess = shininess;
			
			m_BlueCameraUniformBuffer_->SetData(&m_BlueCameraData_, sizeof(tmp_detail::BlueCameraData));
			m_BlueLightUniformBuffer_->SetData(&m_BlueLightData_, sizeof(tmp_detail::BlueLightData));
			
			// drawing m_SquareVA_
			Renderer::Submit(m_SquareVA_);
			
			// cubes
			for (const glm::vec3& current_position : positions)
            {
                glm::mat4 translate_matrix(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    current_position[0], current_position[1], current_position[2], 1);
				m_BlueCameraData_.model_matrix = translate_matrix;
				m_BlueCameraUniformBuffer_->SetData(&m_BlueCameraData_, sizeof(tmp_detail::BlueCameraData));

                Renderer::Submit(m_SquareVA_);
            }
			
			// light source
        	{
            	m_LightSourceShader_->Bind();
				m_LightCameraData_.view_projection_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
				m_LightCameraDataUniformBuffer_->SetData(&m_LightCameraData_, sizeof(tmp_detail::LightCameraData));
				glm::mat4 translate_matrix(1, 0, 0, 0,
            	    0, 1, 0, 0,
            	    0, 0, 1, 0,
            	    light_source_position[0], light_source_position[1], light_source_position[2], 1);
            	m_LightCameraData_.model_matrix = translate_matrix;
				m_LightColor_.light_color = glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]);
				m_LightCameraDataUniformBuffer_->SetData(&m_LightCameraData_, sizeof(tmp_detail::LightCameraData));
				m_LightColorUniformBuffer_->SetData(&m_LightColor_, sizeof(tmp_detail::LightColor));
				Renderer::Submit(m_LightSourceIceCube_);
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
		camera.SetViewPortSize(e.GetWidth(), e.GetHeight());
		return true;
	}
}