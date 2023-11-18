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
		NK_INFO("ExampleLayer::Update");
	}

	void OnEvent(NK::Event& event) override
	{
		NK_TRACE("{0}", event);
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