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
		if (NK::Input::IsKeyPressed(HZ_KEY_TAB))
			NK_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(NK::Event& event) override
	{
		//NK_TRACE("{0}", event);
		if (event.GetEventType() == NK::EventType::KeyPressed)
		{
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				NK_TRACE("Tab key is pressed (event)!");
			NK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
	
};


class Sandbox : public NK::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new NK::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

NK::Application* NK::CreateApplication()
{
	return new Sandbox();
}