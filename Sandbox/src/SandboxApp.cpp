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
		if (NK::Input::IsKeyPressed(NK_KEY_TAB))
			NK_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(NK::Event& event) override
	{
		//NK_TRACE("{0}", event);
		if (event.GetEventType() == NK::EventType::KeyPressed)
		{
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;
			if (e.GetKeyCode() == NK_KEY_TAB)
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
	}

	~Sandbox()
	{

	}

};

NK::Application* NK::CreateApplication()
{
	return new Sandbox();
}