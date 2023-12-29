#include <Nutckracker.h>



class ExampleLayer : public NK::Layer
{
private:
    double m_CurrentMousePos_X_ = 0.0;
    double m_CurrentMousePos_Y_ = 0.0;
    double m_PrevMousePos_X_ = 0.0;
    double m_PrevMousePos_Y_ = 0.0;
	bool MouseRightButtonPressed = false;
	bool MouseRightButtonReleased = false;
	bool MouseLeftButtonPressed = false;
	bool MouseLeftButtonReleased = false;
	bool MousePositionChanged = false;

public:
	ExampleLayer()
		: Layer("Editor Layer")
	{

	}

	void OnUpdate() override
	{
		
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(NK::Event& event) override
	{
		NK::Application& app = NK::Application::Get();
		glm::vec3 MovementDelta = {0.0, 0.0, 0.0};
		glm::vec3 RotationDelta = {0.0, 0.0, 0.0};
		if (event.GetEventType() == NK::EventType::KeyPressed)
		{
			//NK_TRACE("KeyPressed event from Example Layer");
			NK::KeyPressedEvent& e = (NK::KeyPressedEvent&)event;


			
			if (NK::Input::IsKeyPressed(NK_KEY_W))
			{
				MovementDelta.x += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_S))
			{
				MovementDelta.x -= 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_A))
			{
				MovementDelta.y -= 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_D))
			{
				MovementDelta.y += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_E))
			{
				MovementDelta.z += 0.1f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_Q))
			{
				MovementDelta.z -= 0.1f;
			}

			if (NK::Input::IsKeyPressed(NK_KEY_UP))
			{
				RotationDelta.y -= 1.f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_DOWN))
			{
				RotationDelta.y += 1.f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_RIGHT))
			{
				RotationDelta.z -= 1.f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_LEFT))
			{
				RotationDelta.z += 1.f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_P))
			{
				RotationDelta.x += 1.f;
			}
			if (NK::Input::IsKeyPressed(NK_KEY_O))
			{
				RotationDelta.x -= 1.f;
			}
		}

		if (event.GetEventType() == NK::EventType::MouseButtonPressed)
		{
			NK::MouseButtonPressedEvent& e = (NK::MouseButtonPressedEvent&)event;
		    if (e.GetMouseButton() == NK_MOUSE_BUTTON_RIGHT)
			{
				MouseRightButtonPressed = true;
			}
			if (e.GetMouseButton() == NK_MOUSE_BUTTON_LEFT)
			{
				MouseLeftButtonPressed = true;
			}
		}

		if (event.GetEventType() == NK::EventType::MouseButtonReleased)
		{
			NK::MouseButtonReleasedEvent& e = (NK::MouseButtonReleasedEvent&)event;
		    if (e.GetMouseButton() == NK_MOUSE_BUTTON_RIGHT)
			{
				MouseRightButtonPressed = false;
			}
			if (e.GetMouseButton() == NK_MOUSE_BUTTON_LEFT)
			{
				MouseLeftButtonPressed = false;
			}
		}

		if (event.GetEventType() == NK::EventType::MouseMoved)
		{
			NK::MouseMovedEvent& e = (NK::MouseMovedEvent&)event;
		    m_PrevMousePos_X_ = m_CurrentMousePos_X_;
			m_PrevMousePos_Y_ = m_CurrentMousePos_Y_;
			
			m_CurrentMousePos_X_ = e.GetX();
			m_CurrentMousePos_Y_ = e.GetY();
		}

		NK::Camera* camera = app.GetCamera();
		if (MouseRightButtonPressed)
		{
			if (MouseLeftButtonPressed)
			{
				camera->MoveRight(static_cast<float>(m_CurrentMousePos_X_ - m_PrevMousePos_X_) / 100.f);
				camera->MoveUp(static_cast<float>(m_CurrentMousePos_Y_ - m_PrevMousePos_Y_) / 100.f);
			}
			else
			{
				RotationDelta.z += static_cast<float>(m_CurrentMousePos_X_ - m_PrevMousePos_X_) / 5.0f;
				RotationDelta.y -= static_cast<float>(m_CurrentMousePos_Y_ - m_PrevMousePos_Y_) / 5.0f;
			}

		}

		camera->AddMovementAndRotation(MovementDelta, RotationDelta);

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
