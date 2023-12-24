#include <Nutckracker.h>



class ExampleLayer : public NK::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//NK_INFO("ExampleLayer::Update");
		//if (NK::Input::IsKeyPressed(NK_KEY_TAB))
			//NK_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(NK::Event& event) override
	{
		//NK_TRACE("{0}", event);
		if (event.GetEventType() == NK::EventType::KeyPressed)
		{
			NK_TRACE("KeyPressed event from Example Layer");
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;
			if (e.GetKeyCode() == NK_KEY_TAB)
				NK_TRACE("Tab key is pressed (event)!");

			NK::Application& app = NK::Application::Get();
			
			if (NK::Input::IsKeyPressed(NK_KEY_W))
			{
				app.GetCameraPosition()[2] -= 0.01f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_S))
			{
				app.GetCameraPosition()[2] += 0.01f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_A))
			{
				app.GetCameraPosition()[0] -= 0.01f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_D))
			{
				app.GetCameraPosition()[0] += 0.01f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_E))
			{
				app.GetCameraPosition()[1] += 0.01f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_Q))
			{
				app.GetCameraPosition()[1] -= 0.01f;
			}

			if (NK::Input::IsKeyPressed(NK_KEY_UP))
			{
				app.GetCameraRotation()[0] += 0.5f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_DOWN))
			{
				app.GetCameraRotation()[0] -= 0.5f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_RIGHT))
			{
				app.GetCameraRotation()[1] -= 0.5f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_LEFT))
			{
				app.GetCameraRotation()[1] += 0.5f;
			}
		}
	}
	
};


class Sandbox : public NK::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

NK::Application* NK::CreateApplication()
{
	return new Sandbox();
}