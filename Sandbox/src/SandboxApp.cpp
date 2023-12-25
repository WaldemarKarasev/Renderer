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

			bool MoveCamera = false;
			glm::vec3 MovementDelta = {0.0, 0.0, 0.0};
			glm::vec3 RotationDelta = {0.0, 0.0, 0.0};

			NK::Application& app = NK::Application::Get();
			
			if (NK::Input::IsKeyPressed(NK_KEY_W))
			{
				MovementDelta.x += 0.1f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_S))
			{
				MovementDelta.x -= 0.1f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_A))
			{
				MovementDelta.y -= 0.1f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_D))
			{
				MovementDelta.y += 0.1f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_E))
			{
				MovementDelta.z += 0.1f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_Q))
			{
				MovementDelta.z -= 0.1f;
				MoveCamera = true;
			}

			if (NK::Input::IsKeyPressed(NK_KEY_UP))
			{
				RotationDelta.y -= 1.f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_DOWN))
			{
				RotationDelta.y += 1.f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_RIGHT))
			{
				RotationDelta.z -= 1.f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_LEFT))
			{
				RotationDelta.z += 1.f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_P))
			{
				RotationDelta.x += 1.f;
				MoveCamera = true;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_O))
			{
				RotationDelta.x -= 1.f;
				MoveCamera = true;
			}

			if (MoveCamera)
			{
				app.GetCamera()->AddMovementAndRotation(MovementDelta, RotationDelta);
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
