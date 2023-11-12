#include <Nutckracker.h>

class Sandbox : public NK::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

NK::Application* NK::CreateApplication()
{
	return new Sandbox();
}